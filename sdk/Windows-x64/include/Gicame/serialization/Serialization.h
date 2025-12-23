#ifndef __GICAME__SERIALIZATION__SERIALIZATION_H__
#define __GICAME__SERIALIZATION__SERIALIZATION_H__


#include "../common.h"
#include <string>
#include <stdexcept>


namespace Gicame::Serialization {

	class SerializationError : public std::runtime_error {
	public:
		inline SerializationError(const std::string& msg) : std::runtime_error(msg) {}
		inline SerializationError(const char* msg) : std::runtime_error(msg) {}
	};

};


#endif
