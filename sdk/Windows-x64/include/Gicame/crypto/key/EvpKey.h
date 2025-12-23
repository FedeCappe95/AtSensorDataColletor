#ifndef __GICAME__EVPKEY_H__
#define __GICAME__EVPKEY_H__


#include <memory>
#include <vector>
#include "../../common.h"


struct evp_pkey_st;


namespace Gicame::Crypto {

	class EvpKey {

	private:
		std::shared_ptr<evp_pkey_st> sp;
		EvpKey(evp_pkey_st* evpKeyLegacy);

	public:
		evp_pkey_st* get();
		GICAME_CRYPTO_API std::vector<byte_t> publicKeyToPem() const;
		GICAME_CRYPTO_API std::vector<byte_t> privateKeyToPem() const;
		GICAME_CRYPTO_API void publicKeySaveToPemFile(const std::string& path) const;
		GICAME_CRYPTO_API void privateKeySaveToPemFile(const std::string& path) const;
		GICAME_CRYPTO_API size_t maxOutputBufferSize() const;

	public:
		static EvpKey fromOpenSslEvpKey(evp_pkey_st* evpKeyLegacy);
		GICAME_CRYPTO_API static EvpKey newEmptyEvpKey();
		GICAME_CRYPTO_API static EvpKey fromPublicKeyPem(const byte_t* data, const size_t size);
		GICAME_CRYPTO_API static EvpKey fromPublicKeyPem(const std::string& data);
		GICAME_CRYPTO_API static EvpKey fromPublicKeyPem(const std::vector<byte_t>& data);
		GICAME_CRYPTO_API static EvpKey fromPrivateKeyPem(const byte_t* data, const size_t size);
		GICAME_CRYPTO_API static EvpKey fromPrivateKeyPem(const std::string& data);
		GICAME_CRYPTO_API static EvpKey fromPrivateKeyPem(const std::vector<byte_t>& data);
		GICAME_CRYPTO_API static EvpKey fromPublicKeyPemFile(const std::string& path);
		GICAME_CRYPTO_API static EvpKey fromPrivateKeyPemFile(const std::string& path);

	};

};


#endif
