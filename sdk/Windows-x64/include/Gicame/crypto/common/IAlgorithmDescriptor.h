#ifndef __GICAME__IALGORITHMDESCRIPTO_H_
#define __GICAME__IALGORITHMDESCRIPTO_H_


#include "../../common.h"
#include <stdint.h>


namespace Gicame::Crypto {

	class GICAME_CRYPTO_API IAlgorithmDescriptor {

	public:
        virtual size_t keySize() const = 0;

    protected:
		constexpr IAlgorithmDescriptor() {};

    };

};


#endif
