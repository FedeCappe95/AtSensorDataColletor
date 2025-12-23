#ifndef __GICAME__HMACCALCULATOR_H__
#define __GICAME__HMACCALCULATOR_H__


#include <vector>
#include <string.h>
#include "../../common.h"
#include "HashAlgorithm.h"
#include "../key/SymmetricKey.h"


struct hmac_ctx_st;


namespace Gicame::Crypto {

	class GICAME_CRYPTO_API HmacCalculator {

	private:
		SymmetricKey key;
		const HashAlgorithm hashAlgorithm;
		hmac_ctx_st* ctx;

	public:
		HmacCalculator(const SymmetricKey& key, const HashAlgorithm hashAlgorithm);
		~HmacCalculator();
		void update(const std::vector<byte_t>& source);
		void update(const void* source, const size_t size);
		std::vector<byte_t> finalize();
		size_t finalize(void* dest);

	public:
		static std::vector<byte_t> hmac(const SymmetricKey& key, const HashAlgorithm hashAlgorithm, const void* source, const size_t size);
		static size_t hmac(const SymmetricKey& key, const HashAlgorithm hashAlgorithm, const void* source, const size_t size, void* out);

	};

};


#endif
