#include <iostream>
#include <random>
#include <string>
#include <thread>

#include "absl/strings/str_format.h"
#include <gflags/gflags.h>
#include <grpc/support/log.h>
#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <tbb/concurrent_queue.h>

#include "../echo_server/echo.grpc.pb.h"
#include "client_async.h"

DEFINE_string(echo_client_async_server_address, "", "");
DEFINE_uint64(echo_client_async_max_inflight, 64, "");

namespace
{
    class EchoClientAsync
    {
    public:
        using EchoResponseCallback = void (*)(const grpc::Status&, const gcppg::echo::EchoResponse&);

    public:
        EchoClientAsync(const std::shared_ptr<grpc::Channel>& channel, size_t maxInflight)
            : stub(gcppg::echo::Echo::NewStub(channel))
        {
            inflightQueue.set_capacity(maxInflight);
            startThreads();
        }

        ~EchoClientAsync()
        {
            stopThreads();
        }

        void SendRandom(size_t messageLength, EchoResponseCallback cb)
        {
            inflightQueue.push(nullptr);

            AsyncSendRequest *sendRequest = new AsyncSendRequest(cb);
            sendRequest->Request.set_message(genRandomString(messageLength));

            auto asyncResponse = stub->AsyncSend(&sendRequest->Context, sendRequest->Request, &asyncSendCompletionQueue);
            asyncResponse->Finish(&sendRequest->Response, &sendRequest->Status, sendRequest->Key);
        }

    private:
        struct AsyncSendRequest
        {
            explicit AsyncSendRequest(EchoResponseCallback cb)
                : Key(this)
                , Callback(cb)
            {
            }

            static AsyncSendRequest *Detag(void *tag)
            {

                return static_cast<EchoClientAsync::AsyncSendRequest *>(tag);
            }

            void OnComplete()
            {
                Callback(Status, Response);
                delete this;
            }

            void *Key;
            grpc::Status Status;
            grpc::ClientContext Context;
            gcppg::echo::EchoRequest Request;
            gcppg::echo::EchoResponse Response;
            EchoResponseCallback Callback;
        };

    private:
        std::string genRandomString(size_t length) {
            static const char alphanum[] =
                "0123456789"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz";

            std::string s;
            s.reserve(length);

            for (size_t i = 0; i < length; ++i) {
                s.push_back(alphanum[rand() % (sizeof(alphanum) - 1)]);
            }

            return s;
        }

        void asyncSendResponseProcessor()
        {
            bool ok = false;
            void *receivedTag;
            void *inflightDummy;

            while (true)
            {
                bool shouldContinue = asyncSendCompletionQueue.Next(&receivedTag, &ok);
                if (!shouldContinue || !ok)
                {
                    break;
                }

                auto request = AsyncSendRequest::Detag(receivedTag);
                request->OnComplete();

                inflightQueue.pop(inflightDummy);
            }
        }

        void startThreads()
        {
            asyncSendThread = std::thread(&EchoClientAsync::asyncSendResponseProcessor, this);
        }

        void stopThreads()
        {
            asyncSendCompletionQueue.Shutdown();
            asyncSendThread.join();
        }

    private:
        std::unique_ptr<gcppg::echo::Echo::Stub> stub;
        tbb::concurrent_bounded_queue<void *> inflightQueue;

        grpc::CompletionQueue asyncSendCompletionQueue;
        std::thread asyncSendThread;
    };
} // anonymous namespace

namespace GCPPG
{
    int mainEchoClientAsync(int argc, char** argv)
    {
        EchoClientAsync client(
            grpc::CreateChannel(FLAGS_echo_client_async_server_address, grpc::InsecureChannelCredentials()),
            FLAGS_echo_client_async_max_inflight
        );

        auto responseCb = [] (const grpc::Status& status, const gcppg::echo::EchoResponse& response)
        {
            if (status.ok())
            {
                std::cout << response.message() << std::endl;
            }
            else
            {
                std::cerr << "error=" << status.error_message() << std::endl;
            }
        };

        while (true)
        {
            try
            {
                client.SendRandom(32, responseCb);
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << std::endl;
            }
        }

        return 0;
    }
} // namespace GCPPG
