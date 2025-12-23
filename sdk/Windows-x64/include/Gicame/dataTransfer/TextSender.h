#ifndef __GICAME__TEXTSENDER_H__
#define __GICAME__TEXTSENDER_H__


#include "../common.h"
#include "../interfaces/ISender.h"
#include <string>


namespace Gicame {

	/**
	 * Send text to the other peer. The other peer does not have to know the text length a priori:
	 * this information is sent before the data itself.
	 */
	class TextSender {

	private:
		ISender* sender;

	public:
		TextSender(ISender* sender);
		virtual ~TextSender() = default;
		size_t sendText(const std::string_view& s);
		bool isSenderConnected() const;

	};


	/*
	 * Inline implementation
	 */

	 inline TextSender::TextSender(ISender* sender) : sender(sender) {}

	 inline size_t TextSender::sendText(const std::string_view& s) {
		 const uint64_t sizeToSend = (uint64_t)s.size();
		 size_t sentBytes = 0;
		 const byte_t* bufferToSend = (byte_t*)(&sizeToSend);
		 do {
			 sentBytes += sender->send(bufferToSend + sentBytes, sizeof(sizeToSend) - sentBytes);
		 } while (sentBytes < sizeof(sizeToSend));

		 sentBytes = 0;
		 bufferToSend = (byte_t*)s.data();
		 do {
			 sentBytes += sender->send(bufferToSend + sentBytes, s.size() - sentBytes);
		 } while(sentBytes < s.size());

		 return sentBytes;
	 }

	 inline bool TextSender::isSenderConnected() const {
		 return sender->isSenderConnected();
	 }

};


#endif
