#ifndef __GICAME__X509STORE_H__
#define __GICAME__X509STORE_H__


#include <string>
#include <set>
#include <vector>
#include <memory>
#include "../../common.h"
#include "../../utils/NotCopyable.h"
#include "X509Certificate.h"


struct x509_store_st;
struct x509_st;


namespace Gicame::Crypto {

    class X509Store {

        NOT_COPYABLE(X509Store);

    private:
        x509_store_st* nativeStore;
        std::set< std::shared_ptr<x509_st> > certSet;

    public:
        GICAME_CRYPTO_API X509Store();
        GICAME_CRYPTO_API ~X509Store();
        GICAME_CRYPTO_API X509Store& addCertificate(X509Certificate& certificate);
        GICAME_CRYPTO_API X509Store& addCrlFromPemFile(const std::string& filePath);
        GICAME_CRYPTO_API bool verify(X509Certificate& certificate);

    };

};


#endif
