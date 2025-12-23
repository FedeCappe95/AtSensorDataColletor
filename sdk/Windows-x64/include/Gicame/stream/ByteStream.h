#ifndef __GICAME__BYTESTREAM_H__
#define __GICAME__BYTESTREAM_H__


#include "../common.h"
#include <ostream>
#include <istream>


namespace Gicame::Stream {

	using ByteOStream = std::basic_ostream<byte_t, std::char_traits<byte_t>>;
	using ByteIStream = std::basic_istream<byte_t, std::char_traits<byte_t>>;

};


#endif
