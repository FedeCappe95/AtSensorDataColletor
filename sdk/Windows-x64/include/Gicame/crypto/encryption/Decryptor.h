#ifndef __GICAME__DECRYPTOR_H__
#define __GICAME__DECRYPTOR_H__


#include <vector>
#include "common.h"
#include "EncryptionAlgorithm.h"
#include "../key/SymmetricKey.h"


namespace Gicame::Crypto {

	class GICAME_CRYPTO_API Decryptor {

	private:
		const SymmetricKey key;
		void* ctx;
		size_t plaintextLen;
		const EncryptionAlgorithm algorithm;

	public:
		Decryptor(const SymmetricKey& key, const EncryptionAlgorithm algorithm, const byte_t* iv);
		~Decryptor();
		size_t getPlaintextLen();
		size_t update(const void* source, const size_t sourceSize, void* out);
		std::vector<byte_t> update(const void* source, const size_t sourceSize);
		size_t finalize(void* out);
		std::vector<byte_t> finalize();

	public:
		static size_t decrypt(SymmetricKey key, const EncryptionAlgorithm algorithm, const byte_t* iv, const void* source, const size_t sourceSize, void* dest);
		static std::vector<byte_t> decrypt(SymmetricKey key, const EncryptionAlgorithm algorithm, const byte_t* iv, const void* source, const size_t sourceSize);

	};

};


#endif
