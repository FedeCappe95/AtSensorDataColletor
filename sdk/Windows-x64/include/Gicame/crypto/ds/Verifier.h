
#ifndef __GICAME__VERIFIER_H__
#define __GICAME__VERIFIER_H__


#include <vector>
#include "../../common.h"
#include "../key/EvpKey.h"


#ifndef GICAME_CRYPTO_EXPORTS
struct EVP_MD;
struct EVP_MD_CTX;
#endif


namespace Gicame::Crypto {

	class GICAME_CRYPTO_API Verifier {

	private:
		EVP_MD_CTX* ctx;
		const EVP_MD* digestAlg;

	public:
		Verifier();
		~Verifier();
		void update(const std::vector<byte_t>& source);
		void update(const void* source, const size_t size);
		bool finalize(EvpKey pubKey, const std::vector<byte_t>& sign);
		bool finalize(EvpKey pubKey, const void* sign, const size_t size);

	public:
		static bool verify(EvpKey pubKey, const std::vector<byte_t>& source, const std::vector<byte_t>& sign);
		static bool verify(EvpKey pubKey, const void* source, const size_t sourceSize, const void* sign, const size_t signSize);

	};

};


#endif
