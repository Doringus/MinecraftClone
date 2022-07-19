#pragma once

/*
 * Inspired by Sean Parent (Better Code: Concurrency) 
 * And MIPT concurrency course 
*/

#include <deque>
#include <functional>
#include <mutex>
#include <condition_variable>

namespace utils {

	using Task = std::function<void()>;

	class BlockingQueue {
	public:

		bool tryPop(Task& task) {
			std::unique_lock lock{ m_Mutex, std::defer_lock };
			if (!lock.try_lock() || m_Tasks.empty()) {
				return false;
			}
			task = std::move(m_Tasks.front());
			m_Tasks.pop_front();
			return true;
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
			return true
		}

	private:
		std::deque<Task> m_Tasks;
		std::mutex m_Mutex;
		std::condition_variable m_NotEmpty;
	};

}