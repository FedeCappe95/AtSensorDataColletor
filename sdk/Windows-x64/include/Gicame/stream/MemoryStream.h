#ifndef __GICAME__MEMORYSTREAM_H__
#define __GICAME__MEMORYSTREAM_H__


#include "../common.h"
#include <ostream>
#include <istream>
#include <type_traits>
#include <string>  // std::char_traits


#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif



namespace Gicame::Stream {

	template<typename ElemType>
	class MemoryStreamBuf : public std::basic_streambuf<ElemType, std::char_traits<ElemType>> {

	public:
		using _Traits = std::char_traits<ElemType>;
		using int_type = typename _Traits::int_type;
		using pos_type = typename _Traits::pos_type;
		using off_type = typename _Traits::off_type;

	protected:
		ElemType* buffer;
		std::streamsize bufferSize;
		std::streamsize pos;

	public:
		constexpr MemoryStreamBuf(ElemType* buffer, const std::streamsize bufferSize) : buffer{ buffer }, bufferSize{ bufferSize }, pos { 0 } {}

	protected:
		virtual std::streamsize xsputn(const ElemType* ptr, std::streamsize count) override final {
			const std::streamsize toWrite = std::min(count, bufferSize - pos);
			for (std::streamsize i = 0; i < toWrite; ++i)
				buffer[pos++] = ptr[i];
			return toWrite;
		}

		virtual std::streamsize xsgetn(ElemType* ptr, std::streamsize count) override final {
			const std::streamsize toRead = std::min(count, bufferSize - pos);
			for (std::streamsize i = 0; i < toRead; ++i)
				ptr[i] = buffer[pos++];
			return toRead;
		}

		virtual int_type overflow(int_type c = EOF) override final {
			if (pos >= bufferSize)
				return EOF;
			buffer[pos++] = ElemType(c);
			return c;  // Returns unspecified value not equal to Traits::eof() on success, Traits::eof() on failure.
		}

		virtual int_type underflow() override final {
			if (pos >= bufferSize)
				return EOF;
			return int_type(buffer[pos++]);
		}

		virtual int sync() override final {
			return 0;
		}

		//virtual std::basic_streambuf* setbuf(ElemType* buffer, std::streamsize bufferSize) override final {
		//	this->buffer = buffer;
		//	this->bufferSize = bufferSize;
		//	pos = 0;
		//	return this;
		//}

		//virtual std::streamsize seekpos(std::streamsize pos, std::ios_base::openmode which = std::ios_base::in | std::ios_base::out) override final {
		//	return pos;
		//}

		virtual pos_type seekoff(off_type, std::ios_base::seekdir, std::ios_base::openmode = std::ios_base::in | std::ios_base::out) override final {
			return pos_type(pos);
		}

	};

	template<typename ElemType>
	class MemoryOStream : public std::basic_ostream<ElemType, std::char_traits<ElemType>> {

	private:
		MemoryStreamBuf<ElemType> memBuff;

	public:
		inline MemoryOStream(ElemType* buffer, const std::streamsize bufferSize) :
			memBuff(buffer, bufferSize),
			std::basic_ostream<ElemType, std::char_traits<ElemType>>((std::basic_streambuf<ElemType, std::char_traits<ElemType>>*)(&memBuff)) {}
		virtual ~MemoryOStream() {}

	};

	template<typename ElemType>
	class MemoryIStream : public std::basic_istream<ElemType, std::char_traits<ElemType>> {

	private:
		MemoryStreamBuf<ElemType> memBuff;

	public:
		inline MemoryIStream(const ElemType* buffer, const std::streamsize bufferSize) :
			memBuff((ElemType*)buffer, bufferSize),
			std::basic_istream<ElemType, std::char_traits<ElemType>>((std::basic_streambuf<ElemType, std::char_traits<ElemType>>*)(&memBuff)) {}
		virtual ~MemoryIStream() {}

	};

};


#endif
