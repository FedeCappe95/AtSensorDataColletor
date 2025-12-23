#ifndef __GICAME__ENCRYPTIONALGORITHM_H__
#define __GICAME__ENCRYPTIONALGORITHM_H__


#include "../../common.h"
#include "../common/IAlgorithmDescriptor.h"

#ifdef GICAME_CRYPTO_EXPORTS
#include <openssl/evp.h>
#endif



namespace Gicame::Crypto {

    class GICAME_CRYPTO_API EncryptionAlgorithm : public IAlgorithmDescriptor {

    public:
        enum Value {
    		AES_128_CBC,
    		AES_256_CBC,
    		AES_128_ECB,
    		AES_256_ECB
    	};

    private:
        EncryptionAlgorithm() = delete;

    private:
        Value value;

    public:
    	constexpr EncryptionAlgorithm(const Value value) noexcept : value(value) {}
    	constexpr operator Value() const { return value; }  // Allow switch and comarison
    	constexpr bool operator==(EncryptionAlgorithm other) { return value == other.value; }
    	constexpr bool operator!=(EncryptionAlgorithm other) { return value != other.value; }
        inline size_t keySize() const noexcept override {
            static size_t sizes[] = { 16, 32, 16, 32 };
            return sizes[value];
        }
        inline size_t blockSize() const noexcept {
            static size_t sizes[] = { 16, 32, 16, 32 };
            return sizes[value];
        }
        inline size_t chipertextSize(const size_t plaintextSize) const {
            const size_t bs = blockSize();
            const size_t rem = plaintextSize % bs;
            const size_t cts = rem ? plaintextSize + (bs - rem) : plaintextSize;
            if (unlikely(plaintextSize > cts))
                throw RUNTIME_ERROR("Overflow: plaintextSize too large");
            return cts;
        }

#ifdef GICAME_CRYPTO_EXPORTS
    public:
        inline const EVP_CIPHER* toEvpCipher() const {
            switch (value) {
            case EncryptionAlgorithm::AES_128_CBC:
                return EVP_aes_128_cbc();
            case EncryptionAlgorithm::AES_256_CBC:
                return EVP_aes_256_cbc();
            case EncryptionAlgorithm::AES_128_ECB:
                return EVP_aes_128_ecb();
            case EncryptionAlgorithm::AES_256_ECB:
                return EVP_aes_256_ecb();
            default:
                throw RUNTIME_ERROR("Unrecognized cipherName");
            }
        }
#endif

    };

};


#endif
