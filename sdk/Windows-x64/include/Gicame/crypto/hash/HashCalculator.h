#ifndef __GICAME__HASHCALCULATOR_H__
#define __GICAME__HASHCALCULATOR_H__


#include <vector>
#include <string.h>
#include "../../common.h"
#include "HashAlgorithm.h"


namespace Gicame::Crypto {

	class GICAME_CRYPTO_API HashCalculator {

	private:
		HashCalculator();

	public:
		static std::vector<byte_t> hash(const HashAlgorithm hashAlgorithm, const byte_t* source, const size_t sourceSize);
		static size_t hash(const HashAlgorithm hashAlgorithm, const byte_t* source, const size_t sourceSize, void* out);

	};

};


#endif
