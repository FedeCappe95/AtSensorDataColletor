#ifndef __GICAME__TEXTRECEIVER_H__
#define __GICAME__TEXTRECEIVER_H__


#include "../common.h"
#include "../interfaces/IReceiver.h"
#include <string>
#include <limits>


namespace Gicame {

	/**
	 * Receive text from the other peer. TextReceiver does not have to know the text length a
	 * priori: this information is sent before the data itself.
	 */
	class TextReceiver {

	private:
		IReceiver* receiver;

	public:
		TextReceiver(IReceiver* receiver);
		virtual ~TextReceiver() = default;
		std::string receiveText(const size_t maxTextSize = std::numeric_limits<size_t>::max());
		bool isReceiverConnected() const;

	};


	/*
	 * Inline implementation
	 */

	 inline TextReceiver::TextReceiver(IReceiver* receiver) : receiver(receiver) {}

	 inline std::string TextReceiver::receiveText(const size_t maxTextSize) {
		 uint64_t incomingObjectSize;
		 size_t receivedBytes = 0;
		 do {
			 receivedBytes += receiver->receive((byte_t*)(&incomingObjectSize) + receivedBytes, sizeof(incomingObjectSize) - receivedBytes);
		 } while (receivedBytes < sizeof(incomingObjectSize));

		 if (unlikely(incomingObjectSize > maxTextSize))
			 throw RUNTIME_ERROR("Incoming object size greater then maxTextSize");

		 std::string text((size_t)incomingObjectSize, '\0');

		 receivedBytes = 0;
		 do {
			 receivedBytes += receiver->receive((byte_t*)(text.data()) + receivedBytes, (size_t)incomingObjectSize - receivedBytes);
		 } while (receivedBytes < incomingObjectSize);

		 return text;
	 }

	 inline bool TextReceiver::isReceiverConnected() const {
		 return receiver->isReceiverConnected();
	 }

};


#endif
