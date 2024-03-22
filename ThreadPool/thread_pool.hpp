#ifndef _THREAD_POOL_HPP_
#define _THREAD_POOL_HPP_

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

using std::bind;
using std::condition_variable;
using std::forward;
using std::function;
using std::future;
using std::lock_guard;
using std::make_shared;
using std::mutex;
using std::packaged_task;
using std::shared_ptr;
using std::thread;
using std::unique_lock;
using std::vector;

template <typename T>
class SafeQueue {
private:
	std::queue<T> unsafe_queue;
	std::mutex mtx;

public:
	SafeQueue() {}
	SafeQueue(const SafeQueue &) = delete;
	SafeQueue(SafeQueue &&) = delete;
	SafeQueue &operator=(const SafeQueue &) = delete;
	SafeQueue &operator=(SafeQueue &&) = delete;
	~SafeQueue() {}

public:
	bool empty() {
		lock_guard<mutex> lock(mtx);
		return unsafe_queue.empty();
	}

	void push(T &&data) {
		lock_guard<mutex> lock(mtx);
		unsafe_queue.push(std::forward<T>(data));
	}

	bool pop(T &receiver) {
		lock_guard<mutex> lock(mtx);
		if (unsafe_queue.empty()) return false;
		receiver = unsafe_queue.front();
		unsafe_queue.pop();
		return true;
	}
};

class ThreadPool {
	vector<thread *> pool;
	SafeQueue<function<void()>> work_queue;
	bool shutdown = false;
	mutex worker_lock;
	condition_variable conditional_lock;

public:
	ThreadPool() = delete;
	ThreadPool(const ThreadPool &) = delete;
	ThreadPool(ThreadPool &&) = delete;

	ThreadPool &operator=(const ThreadPool &) = delete;
	ThreadPool &operator=(ThreadPool &&) = delete;

	ThreadPool(const int threads_count)
		: pool(threads_count) {
		auto work = [&]() {
			function<void()> work;
			bool dequeued;
			while (!shutdown) {
				{
					unique_lock<mutex> blocker(worker_lock);

					if (work_queue.empty())
						conditional_lock.wait(blocker, [&]() { return !work_queue.empty() || shutdown; });
					dequeued = work_queue.pop(work);
				}

				if (dequeued) work();
			}
		};

		for (auto &t : pool)
			t = new thread(work);
	}

	void join() {
		shutdown = true;
		conditional_lock.notify_all();
		for (auto &t : pool)
			if (t->joinable())
				t->join();
	}

	~ThreadPool() {
		for (auto &t : pool)
			delete t;
	}

	template <typename Func, typename... Args>
	auto submit(Func &&func, Args &&...args) -> decltype(auto) {
		using return_type = decltype(forward<Func>(func)(forward<Args>(args)...));
		auto task_ptr = make_shared<packaged_task<return_type()>>(bind(forward<Func>(func), forward<Args>(args)...));
		work_queue.push([task_ptr]() { (*task_ptr)(); });
		conditional_lock.notify_one();
		return task_ptr->get_future();
	}

	inline bool finished() {
		return work_queue.empty();
	}
};

#endif