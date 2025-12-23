#ifndef __GICAME__RANDOM_H__
#define __GICAME__RANDOM_H__


#include "common.h"


namespace Gicame::Crypto::Random {

	GICAME_CRYPTO_API byte_t* getRandomBytes(void* buffer, const size_t size);

	template<typename T>
	static inline T getRandom() {
		T result;
		getRandomBytes(&result, sizeof(T));
		return result;
	}

};


#endif
