#ifndef __GICAME__CONCURRENCY__SLSPSPQUEUE_H__
#define __GICAME__CONCURRENCY__SLSPSPQUEUE_H__


#include "../common.h"
#include "../utils/NotCopyable.h"
#include "./SPSCQueue.h"


// Help-functions usefull in some classes
namespace Gicame::Concurrency {

	/**
	 * @brief Implementation of SPSCQueue via spinlocks
	 */
	class SLSPSCQueue : public SPSCQueue {

		NOT_COPYABLE(SLSPSCQueue)

	protected:
		GICAME_API virtual void waitElemPresent(const size_t dataSize) override final;
		GICAME_API virtual void waitFreeSpace(const size_t dataSize) override final;

	public:
		GICAME_API SLSPSCQueue(void* buffer, const size_t capacity, const ConcurrencyRole cr);
		GICAME_API virtual ~SLSPSCQueue() override final;

	};

};

#endif
