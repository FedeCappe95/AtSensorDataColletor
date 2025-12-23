#ifndef __GICAME__RPCCOMMON_H__
#define __GICAME__RPCCOMMON_H__


#include <vector>
#include <functional>
#include "../common.h"
#include "../interfaces/ISerializable.h"
#include "./RpcCommon.h"


namespace Gicame {

    /*
     * Prototypes
     */
    class RpcExecutionRequest;

    /**
     * An RpcFunction is identified by a natural number on 32 bits
     */
    typedef uint32_t FunctionId;

    /**
     * Type of RPC callbacks
     */
    using RpcFunction = std::function<std::vector<byte_t>(RpcExecutionRequest*, const std::vector<byte_t>&)>;

    GICAME_PACK_BEGIN;
    /**
     * Describe a paramenter of a RpcExecutionRequest
     */
    class GICAME_API RpcParamDescriptor {

    public:
        uint64_t size;

    public:
        constexpr RpcParamDescriptor() : size(0) {}
        constexpr RpcParamDescriptor(const uint64_t size) : size(size) {}
        constexpr RpcParamDescriptor(const RpcParamDescriptor& other) : size(other.size) {}
        constexpr void operator=(const RpcParamDescriptor& other) { size = other.size; }

    }
    GICAME_PACK_END;

    GICAME_PACK_BEGIN;
    /**
     * The RpcClient send an execution request to the RpcServer. The request is represented by an
     * instance of RpcExecutionRequest.
     */
    class GICAME_API RpcExecutionRequest {

    public:
        static constexpr uint32_t MAGIC = 0xDEADBEEFu;
        static constexpr uint32_t MAGIC_FOR_EMPTY_REQUESTS = 0xFFFFFFFFu;
        static constexpr uint32_t MAX_PARAM_COUNT = 6u;

    public:
        const uint32_t magic;
        const FunctionId functionId;
        RpcParamDescriptor params[MAX_PARAM_COUNT];
        uint8_t paramCount;

    public:
        RpcExecutionRequest();
        RpcExecutionRequest(const FunctionId functionId, const uint8_t paramCount);
        bool checkIntegrity();
        void serialize(void* outBuffer) const;
        void deserialize(const void* inBuffer);

    }
    GICAME_PACK_END;


    /*
     * Inline implementation
     */

    inline RpcExecutionRequest::RpcExecutionRequest() :
        magic(MAGIC_FOR_EMPTY_REQUESTS),
        functionId(0),
        paramCount(0)
    {}

    inline RpcExecutionRequest::RpcExecutionRequest(const FunctionId functionId, const uint8_t paramCount) :
        magic(MAGIC),
        functionId(functionId),
        paramCount(paramCount)
    {
        if (unlikely(paramCount > MAX_PARAM_COUNT)) {
            throw RUNTIME_ERROR("Invalid paramCount");
        }
        for (uint32_t paramIndex = 0; paramIndex < MAX_PARAM_COUNT; ++paramIndex) {
            params[paramIndex] = RpcParamDescriptor();
        }
    }

    inline bool RpcExecutionRequest::checkIntegrity() {
        return magic == MAGIC && paramCount < MAX_PARAM_COUNT;
    }

    inline void RpcExecutionRequest::serialize(void* outBuffer) const {
        const byte_t* t = (byte_t*)this;
        std::copy(t, t + sizeof(*this), (byte_t*)outBuffer);
    }

    inline void RpcExecutionRequest::deserialize(const void* inBuffer) {
        std::copy((byte_t*)inBuffer, (byte_t*)inBuffer + sizeof(*this), (byte_t*)this);
    }

};


#endif
