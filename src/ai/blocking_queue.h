#ifndef AI_QUEUE_H
#define AI_QUEUE_H

#include <deque>
#include <mutex>
#include <condition_variable>

namespace chess {

template <typename T>
class BlockingQueue {
private:
	std::deque<T> _queue;
	std::mutex _mutex;
	std::condition_variable _not_empty;

public:
	/*! Push
	 * Pushes an element onto the front of the queue and notifies any waiting
	 * threads that there are elements in queue.
	 */
	inline void push(const T& obj) {
		std::unique_lock<std::mutex> lock(_mutex);
		_queue.push_front(obj);
		_not_empty.notify_one();
	}

	/*! Pop
	 * Blocks until there are elements in the queue, and then returns the top
	 * element in the queue.
	 * @return Top element
	 */
	inline void pop(const T& obj) {
		std::unique_lock<std::mutex> lock(_mutex);
		_not_empty.wait_for(_mutex, [&_queue] { return !_queue.empty(); });
		T val = _queue.back();
		_queue.pop_back();
		return val;
	}

	/*! Size
	 * Returns the number of elements in this queue.
	 * @return Number of elements
	 */
	inline int size() const {
		std::unique_lock<std::mutex> loc(_mutex);
		return _queue.size();
	}
};

} // namespace chess

#endif // AI_QUEUE_H
