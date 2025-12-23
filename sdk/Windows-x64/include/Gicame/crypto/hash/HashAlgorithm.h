#ifndef __GICAME__HASHALGORITHM_H__
#define __GICAME__HASHALGORITHM_H__


#include "../../common.h"
#include "../common/IAlgorithmDescriptor.h"

#ifdef GICAME_CRYPTO_EXPORTS
#include <openssl/evp.h>
#endif


namespace Gicame::Crypto {

    class GICAME_CRYPTO_API HashAlgorithm : public IAlgorithmDescriptor {

    public:
    	enum Value {
    		MD5,
    		SHA1,
    		SHA224,
    		SHA256,
    		SHA384,
    		SHA512,
    		RIPEMD160
    	};

    private:
        HashAlgorithm() = delete;

    private:
        Value value;

    public:
    	constexpr HashAlgorithm(const Value value) noexcept : value(value) {}
    	constexpr operator Value() const { return value; }  // Allow switch and comarison
    	constexpr bool operator==(HashAlgorithm other) { return value == other.value; }
    	constexpr bool operator!=(HashAlgorithm other) { return value != other.value; }
        inline size_t hashSize() const noexcept {
            static size_t sizes[] = {16, 20, 28, 32, 48, 64, 20};
            return sizes[value];
        }
        inline size_t keySize() const override {
            return hashSize();
        }

#ifdef GICAME_CRYPTO_EXPORTS
    public:
        inline const EVP_MD* toEvpMd() const {
            switch (value) {
            case HashAlgorithm::MD5:       return EVP_md5();
            case HashAlgorithm::SHA1:      return EVP_sha1();
            case HashAlgorithm::SHA224:    return EVP_sha224();
            case HashAlgorithm::SHA256:    return EVP_sha256();
            case HashAlgorithm::SHA384:    return EVP_sha384();
            case HashAlgorithm::SHA512:    return EVP_sha512();
            case HashAlgorithm::RIPEMD160: return EVP_ripemd160();
            default: throw RUNTIME_ERROR("Unrecognized HashAlgorithm");
            }
        }
#endif

    };

};


#endif
