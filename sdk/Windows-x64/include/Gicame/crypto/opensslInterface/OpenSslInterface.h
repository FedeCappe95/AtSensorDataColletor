#ifndef __GICAME__OPENSSLINTERFACE_H__
#define __GICAME__OPENSSLINTERFACE_H__
#ifdef GICAME_CRYPTO_EXPORTS


#include "common.h"
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <string.h>
#include <vector>


namespace Gicame::Crypto::OpenSslInterface {

    static inline std::vector<byte_t> bioToByteVector(BIO* bio) {
        BUF_MEM* bufMemPtr;
        BIO_get_mem_ptr(bio, &bufMemPtr);
        std::vector<byte_t> ret(bufMemPtr->length);
        memcpy(ret.data(), bufMemPtr->data, bufMemPtr->length);
        return ret;
    }

};


#endif
#endif
