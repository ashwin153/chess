#ifndef AI_BLOCKING_QUEUE_H
#define AI_BLOCKING_QUEUE_H

#include <deque>
#include <mutex>
#include <condition_variable>

namespace chess {

/*!
 * This class represents a blocking queue. A blocking queue solves the producer-
 * consumer problem by only forcing consumers to wait until the queue is
 * non-empty before attempting to retrieve elements from it. It also includes
 * a mutex lock to prevent multiple threads from accessing the queue at the
 * same time. This class is thread-safe.
 */
template <typename T>
class BlockingQueue {
private:
	std::deque<T> _queue;
	std::mutex _mutex;
	std::condition_variable _not_empty;

public:
	/*!
	 * Pushes an element onto the front of the queue and notifies any waiting
	 * threads that there are elements in queue.
	 * @param[in] obj Object to push
	 */
	inline void push(const T& obj) {
		std::unique_lock<std::mutex> lock(_mutex);
		_queue.push_front(obj);
		_not_empty.notify_one();
	}

	/*!
	 * Blocks indefinitely until there are elements in the queue, and then 
	 * returns the pops the top element of the queue.
	 * @return Top element.
	 */
	inline T pop() {
		std::unique_lock<std::mutex> lock(_mutex);
		_not_empty.wait(lock, [this] { return !_queue.empty(); });
		T val = _queue.back();
		_queue.pop_back();
		return val;
	}

	/*!
	 * Blocks indefinitely until there are elements in the queue, and then
	 * peeks at the top element of the queue, without removing it.
	 * @return Top element.
	 */
	inline T peek() {
		std::unique_lock<std::mutex> lock(_mutex);
		_not_empty.wait(lock, [this] { return !_queue.empty(); });
		return _queue.back();
	}

	/*!
	 * Returns the number of elements in this queue.
	 * @return Number of elements.
	 */
	inline int size() const {
		std::unique_lock<std::mutex> loc(_mutex);
		return _queue.size();
	}
};

} // namespace chess

#endif // AI_BLOCKING_QUEUE_H
