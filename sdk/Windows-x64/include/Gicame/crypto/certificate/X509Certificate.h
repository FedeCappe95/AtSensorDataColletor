#ifndef __GICAME__X509CERTIFICATE_H__
#define __GICAME__X509CERTIFICATE_H__


#include <memory>
#include <vector>
#include "../../common.h"
#include "../../utils/NotCopyable.h"
#include "../key/EvpKey.h"


struct x509_st;


namespace Gicame::Crypto {

	class X509Store;

	class X509Certificate {

		friend class X509Store;  // X509Store needs to access ptr

		NOT_COPYABLE(X509Certificate);

	private:
		std::shared_ptr<x509_st> ptr;

	private:
		X509Certificate(x509_st* cert);

	public:
		GICAME_CRYPTO_API std::vector<byte_t> toDer() const;
		GICAME_CRYPTO_API std::vector<byte_t> toPem() const;
		GICAME_CRYPTO_API EvpKey getPublicKey() const;
		GICAME_CRYPTO_API void sign(EvpKey privKey);
		GICAME_CRYPTO_API std::string getSubjectName() const;
		GICAME_CRYPTO_API X509Certificate& setSubjectName(const std::string& field, const std::string& value);
		GICAME_CRYPTO_API X509Certificate& addSubjectName(const std::string& field, const std::string& value);
		GICAME_CRYPTO_API std::string getIssuerName() const;
		GICAME_CRYPTO_API X509Certificate& setIssuerName(const std::string& field, const std::string& value);
		GICAME_CRYPTO_API X509Certificate& addIssuerName(const std::string& field, const std::string& value);

	public:
		GICAME_CRYPTO_API static X509Certificate newEmptyX509Certificate();
		GICAME_CRYPTO_API static X509Certificate fromDer(const std::vector<byte_t>& der);
		GICAME_CRYPTO_API static X509Certificate fromPem(const std::vector<byte_t>& pem);
		GICAME_CRYPTO_API static X509Certificate fromPem(const std::string& pem);
		GICAME_CRYPTO_API static X509Certificate fromPem(const byte_t* pem, const size_t size);
		GICAME_CRYPTO_API static X509Certificate fromDerFile(const std::string& filePath);
		GICAME_CRYPTO_API static X509Certificate fromPemFile(const std::string& filePath);

	};

};


#endif
