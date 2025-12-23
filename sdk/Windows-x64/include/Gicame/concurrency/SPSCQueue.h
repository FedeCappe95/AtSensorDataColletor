#ifndef __GICAME__CONCURRENCY__SPSPQUEUE_H__
#define __GICAME__CONCURRENCY__SPSPQUEUE_H__


#include "../common.h"
#include "../utils/NotCopyable.h"
#include "./ConcurrencyRole.h"
#include "../interfaces/IDataExchanger.h"


// Forward declarations
namespace Gicame::Concurrency::Impl {
	struct CircularBufferDescriptor;
}


namespace Gicame::Concurrency {

	/**
	 * @brief Abstract class for a Single Producer Single Consumer Queue
	 *
	 * Implementation: circular buffer
	 */
	class SPSCQueue : public IDataExchanger {

		NOT_COPYABLE(SPSCQueue)

	protected:  // Protected data members
		Impl::CircularBufferDescriptor* header;
		uint8_t* buffer;
		size_t capacity;

	protected:  // Protected data members
		virtual void waitElemPresent(const size_t dataSize) = 0;
		virtual void waitFreeSpace(const size_t dataSize) = 0;

	public:    // Public methods
		GICAME_API SPSCQueue(void* buffer_, const size_t capacity_, const ConcurrencyRole cr);
		GICAME_API virtual ~SPSCQueue();
		GICAME_API virtual void push(const void* data, size_t dataSize);
		GICAME_API virtual void pop(void* outBuffer, size_t dataSize);
		GICAME_API virtual size_t size();

		// IDataExchanger interface
		virtual size_t send(const void* data, const size_t dataSize) override final;
		virtual bool isSenderConnected() const override final;
		virtual size_t receive(void* outBuffer, const size_t dataSize) override final;
		virtual bool isReceiverConnected() const override final;

	};


	/*
	 * Inline implementation
	 */

	inline size_t SPSCQueue::send(const void* data, const size_t dataSize) {
		push(data, dataSize);
		return dataSize;
	}

	inline bool SPSCQueue::isSenderConnected() const { return true; }

	inline size_t SPSCQueue::receive(void* outBuffer, const size_t dataSize) {
		pop(outBuffer, dataSize);
		return dataSize;
	}

	inline bool SPSCQueue::isReceiverConnected() const { return true; }

};

#endif
