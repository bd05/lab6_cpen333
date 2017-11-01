#ifndef LAB6_DYNAMICORDERQUEUE_H
#define LAB6_DYNAMICORDERQUEUE_H

#include "OrderQueue.h"
#include <deque>
#include <condition_variable>
#include <mutex>

/**
* Dynamically-sized Queue Implementation
*
* Does not block when adding items
*/
class DynamicOrderQueue : public virtual OrderQueue {
	std::deque<Order> buff_;
	std::mutex mutex_;
	std::condition_variable cv_;

public:
	/**
	* Creates the dynamic queue
	*/
	DynamicOrderQueue() :
		buff_(), mutex_(), cv_() {}

	void add(const Order& order) {

		//==================================================
		// TODO: Safely add item to "queue"
		//    - safely add to end of internal queue
		//    - notify others of item availability
		//==================================================
		std::unique_lock<std::mutex> lock(mutex_);
		buff_.push_back(order);
		lock.unlock();
		cv_.notify_one();

	}

	Order get() {

		//==================================================
		// TODO: Safely remove item from "queue"
		//    - wait until internal queue is non-empty
		//    - safely acquire item from internal queue
		//==================================================
		//block consumer if the queue is empty
		// get first item in queue
		std::unique_lock<std::mutex> lock(mutex_);
		cv_.wait(lock, [&]() { return !buff_.empty(); });
		Order out = buff_.front();
		buff_.pop_front();
		lock.unlock();
		return out;
	}
};

#endif //LAB6_DYNAMICORDERQUEUE_H