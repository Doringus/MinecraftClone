#pragma once

#include <thread>
#include <atomic>
#include <vector>
#include <future>
#include <memory>

#include <spdlog/spdlog.h>

#include "blockingqueue.h"

namespace utils {

	template<typename T>
	static inline void waitForAllTasks(const std::vector<std::future<T>>& tasks) {
		for (const auto& task : tasks) {
			task.wait();
		}
	}

	class ThreadPool {
	public:
		ThreadPool(size_t threadsCount = 4) : m_ThreadsCount(threadsCount), m_TaskQueues(threadsCount) {
			for (size_t i = 0; i < threadsCount; ++i) {
				m_Threads.emplace_back([this, i]() {
					workerRoutine(i);
				});
			}
		}

		~ThreadPool() noexcept {
			for (auto& queue : m_TaskQueues) {
				queue.done();
			}
			for (auto& thread : m_Threads) {
				thread.join();
			}
		}

		template <typename Func, typename... Args, typename Return = std::invoke_result_t<std::decay_t<Func>, std::decay_t<Args>...>>
		auto submit(Func&& func, Args&&... args) -> std::future<Return> {
			auto i = m_WorkerIndex++;
			auto task = std::make_shared<std::packaged_task<Return()>>(std::bind_front(std::forward<Func>(func), std::forward<Args>(args)...));
			auto taskWrapper = [task]() {
				(*task)();
			};
			for (size_t n = 0; n < m_ThreadsCount; ++n) {
				if (m_TaskQueues[(i + n) % m_ThreadsCount].tryPush(taskWrapper)) {
					return task->get_future();
				}
			}
			
			m_TaskQueues[i % m_ThreadsCount].push(taskWrapper);
			return task->get_future();
		}

	private:

		void workerRoutine(size_t threadId) {
			while (true) {
				Task task;
				for (size_t i = 0; i < m_ThreadsCount; ++i) {
					if (m_TaskQueues[(threadId + i) % m_ThreadsCount].tryPop(task)) {
						break;
					}
				}
				if (!task && !m_TaskQueues[threadId].pop(task)) {
					break;
				}
				task();
			}
		}

	private:
		size_t m_ThreadsCount;
		std::atomic<size_t> m_WorkerIndex = 0;
		std::vector<std::thread> m_Threads;
		std::vector<BlockingQueue> m_TaskQueues;
	};
	
}