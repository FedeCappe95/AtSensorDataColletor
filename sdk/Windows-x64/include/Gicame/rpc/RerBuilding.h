#ifndef __GICAME__RERBUILDING_H__
#define __GICAME__RERBUILDING_H__


#include "./RpcCommon.h"
#include "../dataSerialization/BinarySerializer.h"
#include "../stream/MemoryStream.h"
#include <tuple>


namespace Gicame::RerBuilding {

    /**
    * Build a RpcExecutionRequest for the following functionId and the following parameters (args).
    * It uses innerBuild.
    */
    template <class... Args>
    inline std::tuple<RpcExecutionRequest, std::vector<byte_t>> build(const BinarySerializer& serializer, const FunctionId functionId, Args... args) {
        constexpr size_t paramCount = sizeof...(args);
        static_assert(paramCount <= RpcExecutionRequest::MAX_PARAM_COUNT, "Too many parameters");

        RpcExecutionRequest rer(functionId, paramCount);

        if constexpr (paramCount == 0) {
            return std::make_tuple(rer, std::vector<byte_t>());
        }
        else {
            const size_t serBufferSizes[] = { serializer.serializedSize(args)... };
            size_t totalSize = 0;
            for (size_t paramIndex = 0; paramIndex < paramCount; ++paramIndex) {
                rer.params[paramIndex].size = serBufferSizes[paramIndex];
                totalSize += serBufferSizes[paramIndex];
            }

            std::vector<byte_t> paramsDump(totalSize);
            Stream::MemoryOStream<byte_t> memoryOStream(paramsDump.data(), paramsDump.size());
            using unpack_t = int[];
            (void)unpack_t {
                (static_cast<void>(serializer.serialize(args, memoryOStream)), 0)..., 0
            };

            return std::make_tuple(rer, paramsDump);
        }
    }

};


#endif
