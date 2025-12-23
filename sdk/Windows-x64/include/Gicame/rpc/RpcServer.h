#ifndef __GICAME__RPCSERVER_H__
#define __GICAME__RPCSERVER_H__


#include <unordered_map>
#include <functional>
#include <vector>
#include "../common.h"
#include "../utils/NotCopyable.h"
#include "../interfaces/IDataExchanger.h"
#include "./RpcCommon.h"
#include "../dataSerialization/BinarySerializer.h"
#include "../stream/ByteStream.h"
#include "../stream/MemoryStream.h"


namespace Gicame {

    class GICAME_API RpcServer {

        NOT_COPYABLE(RpcServer)

    public:  // Public types
        enum class InvalidRequestReason {
            RER_INTEGRITY_CHECK_FAILED, INVALID_FUNCTION_ID, BAD_PARAMETERS
        };
        using InvalidRequestEventHandler = std::function<bool(const RpcExecutionRequest&, const InvalidRequestReason)>;

    private:  // Private types
        /**
         * Describe an RPC Function stored in the RpcServer
         */
        struct RpcFunctionDescriptor {
            RpcFunction rpcFunction;
            FunctionId functionId;
            inline RpcFunctionDescriptor(const RpcFunction rpcFunction, const FunctionId functionId) :
                rpcFunction(rpcFunction), functionId(functionId) {}
        };

    private:  // Private data
        IDataExchanger* dataExchanger;
        BinarySerializer serializer;
#ifdef MSVC
#pragma warning(push)
#pragma warning(disable:4251)
#endif
        std::unordered_map<FunctionId, RpcFunctionDescriptor> funStore;
        InvalidRequestEventHandler invalidRequestEventHandler;
#ifdef MSVC
#pragma warning(pop)
#endif

    private:  // Private methods
        template <class Function, class Result, class... Args>
        void registerFunctionInternal(Function function, const FunctionId functionId);

    public:  // Public methods
        RpcServer(IDataExchanger* dataExchanger, const BinarySerializer& serializer = BinarySerializer());

        /**
         * Add or replace an RpcFunction to the RpcServer
         */
        void registerRpcFunction(RpcFunction rpcFunction, const FunctionId functionId);

        /**
         * Add or replace a function to the RpcServer
         */
        template <class Result, class... Args>
        void registerFunction(Result function(Args...), const FunctionId functionId);

        /**
         * Add or replace a function to the RpcServer
         */
        template <class Result, class... Args>
        void registerFunction(std::function<Result(Args...)> function, const FunctionId functionId);

        /**
         * Process a single execution request
         */
        bool oneShot();

        /**
         * Process execution requests in loop
         */
        void run();

        /**
         * Invalid execution request handler
         */
        void setInvalidRequestEventHandler(InvalidRequestEventHandler& handler);

    };


    /*
     * Inline implementation
     */

    template <class Function, class Result, class... Args>
    inline void RpcServer::registerFunctionInternal(Function function, const FunctionId functionId) {
        registerRpcFunction(
            [=](RpcExecutionRequest*, const std::vector<byte_t>& params) {
                Stream::MemoryIStream<byte_t> memoryIStream(params.data(), (std::streamsize)params.size());
                if constexpr (std::is_same<Result, void>::value) {
                    function(serializer.deserialize<Args>(memoryIStream)...);
                    return std::vector<byte_t>(1, 0x00);
                }
                else {
                    const Result result = function(serializer.deserialize<Args>(memoryIStream)...);
                    return serializer.serialize(result);
                }
            },
            functionId
        );
    }

    template <class Result, class... Args>
    inline void RpcServer::registerFunction(Result function(Args...), const FunctionId functionId) {
        registerFunctionInternal<decltype(function), Result, Args...>(function, functionId);
    }

    template <class Result, class... Args>
    inline void RpcServer::registerFunction(std::function<Result(Args...)> function, const FunctionId functionId) {
        registerFunctionInternal<decltype(function), Result, Args...>(function, functionId);
    }

};


#endif
