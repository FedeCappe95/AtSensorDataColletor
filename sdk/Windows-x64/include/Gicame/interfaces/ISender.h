#ifndef __GICAME__IWRITER_H__
#define __GICAME__IWRITER_H__


#include <vector>
#include "../common.h"


namespace Gicame {

    class GICAME_API ISender {

    public:
        virtual ~ISender() = default;
        virtual size_t send(const void* buffer, const size_t size) = 0;
        virtual size_t send(const std::vector<byte_t>& buffer);
        virtual bool isSenderConnected() const = 0;

    };

    inline size_t ISender::send(const std::vector<byte_t>& buffer) {
        return send(buffer.data(), buffer.size());
    }

};


#endif
