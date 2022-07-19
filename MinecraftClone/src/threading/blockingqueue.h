#pragma once

/*
 * Inspired by Sean Parent (Better Code: Concurrency) 
 * And MIPT concurrency course 
*/

#include <deque>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <utility>

namespace utils {

	using Task = std::function<void()>;

	class BlockingQueue {
	public:

		void done() {
			{
				std::unique_lock lock{ m_Mutex };
				m_IsDone = true;
			}
			m_NotEmpty.notify_all();
		}

		bool pop(Task& task) {
			std::unique_lock lock{ m_Mutex };
			while (m_Tasks.empty() && !m_IsDone) {
				m_NotEmpty.wait(lock);
			}
			if (m_Tasks.empty()) {
				return false;
			}
			takeLocked(task);
			return true;
		}

		bool tryPop(Task& task) {
			std::unique_lock lock{ m_Mutex, std::defer_lock };
			if (!lock.try_lock() || m_Tasks.empty()) {
				return false;
			}
			takeLocked(task);
			return true;
		}

		template <typename T>
		void push(T&& task) {
			{
				std::unique_lock lock{ m_Mutex };
				m_Tasks.emplace_back(std::forward<T>(task));
			}
			m_NotEmpty.notify_one();
		}

		template <typename T>
		bool tryPush(T&& task) {
			{
				std::unique_lock lock{ m_Mutex, std::defer_lock };
				if (!lock.try_lock()) {
					return false;
				}
				m_Tasks.emplace_back(std::forward<T>(task));
			}
			m_NotEmpty.notify_one();
			return true;
		}

	private:

		void takeLocked(Task& task) {
			task = std::move(m_Tasks.front());
			m_Tasks.pop_front();
		}

	private:
		bool m_IsDone;
		std::deque<Task> m_Tasks;
		std::mutex m_Mutex;
		std::condition_variable m_NotEmpty;
	};

}