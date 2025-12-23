#ifndef __GICAME__ENCRYPTOR_H__
#define __GICAME__ENCRYPTOR_H__


#include <vector>
#include "../../common.h"
#include "../encryption/EncryptionAlgorithm.h"
#include "../key/SymmetricKey.h"



namespace Gicame::Crypto {

	class GICAME_CRYPTO_API Encryptor {

	private:
		const SymmetricKey key;
		void* ctx;
		size_t ciphertextLen;
		const EncryptionAlgorithm algorithm;

	public:
		Encryptor(const SymmetricKey& key, const EncryptionAlgorithm algorithm, const byte_t* iv);
		~Encryptor();
		size_t getCiphertextLen();
		size_t update(const void* source, const size_t sourceSize, void* out);
		std::vector<byte_t> update(const void* source, const size_t sourceSize);
		size_t finalize(void* out);
		std::vector<byte_t> finalize();

	public:
		static size_t encrypt(const SymmetricKey& key, const EncryptionAlgorithm algorithm, const byte_t* iv, const void* source, const size_t sourceSize, void* dest);
		static std::vector<byte_t> encrypt(const SymmetricKey& key, const EncryptionAlgorithm algorithm, const byte_t* iv, const void* source, const size_t sourceSize);

	};

};


#endif
