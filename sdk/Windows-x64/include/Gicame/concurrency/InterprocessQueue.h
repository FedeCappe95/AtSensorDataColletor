#ifndef __GICAME__CONCURRENCY__INTERPROCESSQUEUE_H__
#define __GICAME__CONCURRENCY__INTERPROCESSQUEUE_H__


#include "../common.h"
#include "../utils/NotCopyable.h"
#include "./SPSCQueue.h"
#include "../concurrency/Semaphore.h"
#include "../sm/SharedMemory.h"
#include "../interfaces/IDataExchanger.h"
#include "./InterprocessSignal.h"
#include <string>


namespace Gicame::Concurrency::Impl {
	struct CircularBufferDescriptor;
};


namespace Gicame::Concurrency {

	/**
	 * @brief Implementation of InterprocessQueue
	 */
	class InterprocessQueue : public IDataExchanger {

		NOT_COPYABLE(InterprocessQueue)

	private:
		Gicame::Concurrency::Impl::CircularBufferDescriptor* header;
		uint8_t* buffer;
		size_t capacity;
		Gicame::SharedMemory shmem;
		InterprocessSignal dataPresentEvent;
		InterprocessSignal dataFreeEvent;

	private:
		void waitElemPresent(const size_t dataSize);
		void waitFreeSpace(const size_t dataSize);

	public:
		GICAME_API InterprocessQueue(const std::string& name, const size_t capacity, const ConcurrencyRole cr);
		GICAME_API ~InterprocessQueue();
		GICAME_API void push(const void* data, size_t dataSize);
		GICAME_API void pop(void* outBuffer, size_t dataSize);
		GICAME_API size_t size();
		GICAME_API size_t freeSpace();

		// IDataExchanger interface
		virtual size_t send(const void* data, const size_t dataSize) override final;
		virtual bool isSenderConnected() const override final;
		virtual size_t receive(void* outBuffer, const size_t dataSize) override final;
		virtual bool isReceiverConnected() const override final;

	};


	/*
	 * Inline implementation
	 */

	inline size_t InterprocessQueue::send(const void* data, const size_t dataSize) {
		push(data, dataSize);
		return dataSize;
	}

	inline bool InterprocessQueue::isSenderConnected() const { return true; }

	inline size_t InterprocessQueue::receive(void* outBuffer, const size_t dataSize) {
		pop(outBuffer, dataSize);
		return dataSize;
	}

	inline bool InterprocessQueue::isReceiverConnected() const { return true; }

};

#endif
