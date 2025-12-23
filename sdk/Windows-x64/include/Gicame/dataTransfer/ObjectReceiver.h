#ifndef __GICAME__OBJECTRECEIVER_H__
#define __GICAME__OBJECTRECEIVER_H__


#include "../common.h"
#include "../interfaces/IReceiver.h"


namespace Gicame {

	/**
	 * Receive data from the other peer. ObjectReceiver does not have to know the data size a
	 * priori: this information is sent before the data itself.
	 */
	class ObjectReceiver {

	private:
		IReceiver* receiver;

	public:
		ObjectReceiver(IReceiver* receiver);
		virtual ~ObjectReceiver() = default;
		void receiveObject(void* buffer, const size_t bufferSize);
		bool isReceiverConnected() const;

	};


	/*
	 * Inline implementation
	 */

	 inline ObjectReceiver::ObjectReceiver(IReceiver* receiver) : receiver(receiver) {}

	 inline void ObjectReceiver::receiveObject(void* buffer, const size_t bufferSize) {
		 uint64_t incomingObjectSize;
		 size_t receivedBytes = 0;
		 do {
			 receivedBytes += receiver->receive((byte_t*)(&incomingObjectSize) + receivedBytes, sizeof(incomingObjectSize) - receivedBytes);
		 } while (receivedBytes < sizeof(incomingObjectSize));

		 if (unlikely(incomingObjectSize > bufferSize))
			 throw RUNTIME_ERROR("Incoming object size greater then buffer size");

		 receivedBytes = 0;
		 do {
			 receivedBytes += receiver->receive((byte_t*)buffer + receivedBytes, (size_t)incomingObjectSize - receivedBytes);
		 } while (receivedBytes < incomingObjectSize);
	 }

	 inline bool ObjectReceiver::isReceiverConnected() const {
		 return receiver->isReceiverConnected();
	 }

};


#endif
