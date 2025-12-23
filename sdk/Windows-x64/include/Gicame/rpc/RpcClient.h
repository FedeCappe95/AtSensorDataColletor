#ifndef __GICAME__RPCCLIENT_H__
#define __GICAME__RPCCLIENT_H__


#include "../common.h"
#include "../utils/NotCopyable.h"
#include "../interfaces/IDataExchanger.h"
#include "./RpcCommon.h"
#include "./RerBuilding.h"
#include "../dataSerialization/BinarySerializer.h"
#include <vector>


namespace Gicame {

    class GICAME_API RpcClient {

        NOT_COPYABLE(RpcClient)

    private:
        IDataExchanger* dataExchanger;
        BinarySerializer serializer;

    private:
        /**
         * Send an RpcExecutionRequest and its serialized parameters
         */
        std::vector<byte_t> sendExeRequest(const RpcExecutionRequest& rer, const std::vector<byte_t> paramsDump, const size_t replySize);

    public:
        RpcClient(IDataExchanger* dataExchanger, const BinarySerializer& serializer = BinarySerializer());

        /**
         * Build a lamdas to send the RpcExecutionRequests
         */
        template <class RetType, class... Args>
        auto get(const FunctionId functionId);

    };


    /*
     * Inline implementation
     */

    template <class RetType, class... Args>
    inline auto RpcClient::get(const FunctionId functionId) {
        RpcClient* thisRpcClient = this;
        const size_t replySize = std::is_same<RetType, void>::value ? 1u : serializer.serializedSize<RetType>();
        return [=](const Args... args) {
            auto [rer, paramsDump] = Gicame::RerBuilding::build(serializer, functionId, args...);
            const std::vector<byte_t> serializedeReply = thisRpcClient->sendExeRequest(rer, paramsDump, replySize);
            if constexpr (std::is_same<RetType, void>::value)
                return;
            else
                return serializer.deserialize<RetType>(serializedeReply.data(), serializedeReply.size());
        };
    }

};


#endif
