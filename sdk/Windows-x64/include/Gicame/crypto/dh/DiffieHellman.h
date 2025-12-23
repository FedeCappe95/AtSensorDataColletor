#ifndef __GICAME__DIFFIEHELLMAN_H__
#define __GICAME__DIFFIEHELLMAN_H__


#include <vector>
#include <string.h>
#include "common.h"
#include "../hash/HashAlgorithm.h"


#ifndef GICAME_CRYPTO_EXPORTS
struct DH;
#endif


namespace Gicame::Crypto {

	class GICAME_CRYPTO_API DiffieHellman {

	private:
		DH* dh;

	public:
		DiffieHellman();
		~DiffieHellman();
		void createSession(const std::vector<byte_t>& generator, const std::vector<byte_t>& primeModulus);
		size_t getSize();
		std::vector<byte_t> getPublicComponent();
		std::vector<byte_t> computeSharedSecret(const std::vector<byte_t>& otherPeerPublicComponent);
		std::vector<byte_t> computeSharedSecret(const std::vector<byte_t>& otherPeerPublicComponent, const HashAlgorithm hashAlgorithm);

	};

};


#endif
