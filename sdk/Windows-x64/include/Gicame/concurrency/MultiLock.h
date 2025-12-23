#ifndef __GICAME__MULTILOCK_H__
#define __GICAME__MULTILOCK_H__


#include <stdint.h>
#include <limits>
#include "../common.h"


namespace Gicame {

	/**
	 * @brief Protect a lock from multiple acquire/release operation.
	 * 
	 * This non-thread safe class wraps a lock and protectes it from multiple
	 * acquire/release operation.
	 * Each thread or process should have its own MultiLock instance.
	 * MultiLock satisfies the Lockable requirements.
	 */
	template <typename Lock>
	class MultiLock {

	private:
		size_t counter;
		Lock& theLock;

	public:
		MultiLock(Lock& theLock_);
		MultiLock(const MultiLock<Lock>&) = delete;
		~MultiLock() = default;
		void lock();
		void unlock();
		bool try_lock();  // snake_case to match std lib

	};


	template <typename Lock>
	inline MultiLock<Lock>::MultiLock(Lock& theLock_) : counter(0), theLock(theLock_) {}

	template <typename Lock>
	inline void MultiLock<Lock>::lock() {
		if (unlikely(counter == std::numeric_limits<size_t>::max()))
			throw RUNTIME_ERROR("counter overflow");

		if (counter == 0u)
			theLock.lock();

		++counter;
	}

	template <typename Lock>
	inline void MultiLock<Lock>::unlock() {
		if (unlikely(counter == 0u))
			throw RUNTIME_ERROR("counter underflow");

		if (counter == 1u)
			theLock.unlock();

		--counter;
	}

	template <typename Lock>
	inline bool MultiLock<Lock>::try_lock() {
		if (unlikely(counter == std::numeric_limits<size_t>::max()))
			throw RUNTIME_ERROR("counter overflow");

		bool success;
		if (counter == 0u) {
			success = theLock.try_lock();
			if (success)
				counter = 1u;
		}
		else {
			success = true;
			++counter;
		}

		return success;
	}

};


#endif
