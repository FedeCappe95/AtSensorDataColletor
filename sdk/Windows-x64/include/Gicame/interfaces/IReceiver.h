#ifndef __GICAME__IRECEIVER_H__
#define __GICAME__IRECEIVER_H__


#include <vector>
#include "../common.h"


namespace Gicame {

    class GICAME_API IReceiver {

    public:
        virtual ~IReceiver() = default;
        virtual size_t receive(void* buffer, const size_t size) = 0;
        virtual std::vector<byte_t> receive(const size_t size);
        virtual bool isReceiverConnected() const = 0;

    };

    inline std::vector<byte_t> IReceiver::receive(const size_t size) {
        std::vector<byte_t> ret(size, 0u);
        const size_t receivedBytes = receive(ret.data(), size);
        ret.resize(receivedBytes);
        return ret;
    }

};


#endif
