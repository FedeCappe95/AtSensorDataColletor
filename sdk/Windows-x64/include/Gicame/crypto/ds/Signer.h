#ifndef __GICAME__SIGNER_H__
#define __GICAME__SIGNER_H__


#include <vector>
#include <string.h>
#include "../../common.h"
#include "../key/EvpKey.h"


#ifndef GICAME_CRYPTO_EXPORTS
struct EVP_MD;
struct EVP_MD_CTX;
#endif


namespace Gicame::Crypto {

	class GICAME_CRYPTO_API Signer {

	private:
		const EVP_MD* digestAlg;
		EVP_MD_CTX* ctx;

	public:
		Signer();
		~Signer();
		void update(const std::vector<byte_t>& source);
		void update(const void* source, const size_t sourceSize);
		std::vector<byte_t> finalize(EvpKey privKey);
		size_t finalize(EvpKey privKey, void* outSign);

	public:
		static std::vector<byte_t> sign(EvpKey privKey, const void* source, const size_t sourceSize);
		static size_t sign(EvpKey privKey, const void* source, const size_t sourceSize, void* dest);

	};

};


#endif
