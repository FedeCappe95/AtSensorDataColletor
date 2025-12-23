#ifndef __GICAME__OBJECTSENDER_H__
#define __GICAME__OBJECTSENDER_H__


#include "../common.h"
#include "../interfaces/ISender.h"


namespace Gicame {

	/**
	 * Send data to the other peer. The other peer does not have to know the data size a priori:
	 * this information is sent before the data itself.
	 */
	class ObjectSender {

	private:
		ISender* sender;

	public:
		ObjectSender(ISender* sender);
		virtual ~ObjectSender() = default;
		size_t sendObject(const void* buffer, const size_t size);
		bool isSenderConnected() const;

	};


	/*
	 * Inline implementation
	 */

	 inline ObjectSender::ObjectSender(ISender* sender) : sender(sender) {}

	 inline size_t ObjectSender::sendObject(const void* buffer, const size_t size) {
		 const uint64_t sizeToSend = (uint64_t)size;
		 size_t sentBytes = 0;
		 const byte_t* bufferToSend = (byte_t*)(&sizeToSend);
		 do {
			 sentBytes += sender->send(bufferToSend + sentBytes, sizeof(sizeToSend) - sentBytes);
		 } while (sentBytes < sizeof(sizeToSend));

		 sentBytes = 0;
		 bufferToSend = (byte_t*)buffer;
		 do {
			 sentBytes += sender->send(bufferToSend + sentBytes, size - sentBytes);
		 } while(sentBytes < size);

		 return sentBytes;
	 }

	 inline bool ObjectSender::isSenderConnected() const {
		 return sender->isSenderConnected();
	 }

};


#endif
