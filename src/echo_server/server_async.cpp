#include "absl/strings/str_format.h"
#include <gflags/gflags.h>
#include <grpcpp/completion_queue.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include "echo.grpc.pb.h"
#include "server_async.h"

DEFINE_uint32(echo_server_async_port, 0, "");

namespace
{
    class RequestHandler
    {
    public:
        RequestHandler(gcppg::echo::Echo::AsyncService *service, grpc::ServerCompletionQueue *cq)
            : service_(service)
            , cq_(cq)
            , responseWriter_(&ctx_)
            , status_(CREATE)
        {
            Handle();
        }

        void Handle()
        {
            if (status_ == CREATE)
            {
                status_ = PROCESS;
                service_->RequestSend(&ctx_, &request_, &responseWriter_, cq_, cq_, this);
            }
            else if (status_ == PROCESS)
            {
                new RequestHandler(service_, cq_);
                response_.set_message(request_.message());
                status_ = FINISH;
                responseWriter_.Finish(response_, grpc::Status::OK, this);
            }
            else
            {
                GPR_ASSERT(status_ == FINISH);
                delete this;
            }
        }

    private:
        gcppg::echo::Echo::AsyncService *service_;
        grpc::ServerCompletionQueue *cq_;
        grpc::ServerContext ctx_;

        gcppg::echo::EchoRequest request_;
        gcppg::echo::EchoResponse response_;

        grpc::ServerAsyncResponseWriter<gcppg::echo::EchoResponse> responseWriter_;

        enum Status { CREATE, PROCESS, FINISH };
        Status status_;
    };

    class Server
    {
    public:
        explicit Server(std::string address)
            : address_(std::move(address))
        {
        }

        ~Server()
        {
            server_->Shutdown();
            cq_->Shutdown();
        }

        void Run()
        {
            Init();
            DoRun();
        }

    private:
        void Init()
        {
            grpc::ServerBuilder builder;
            builder.AddListeningPort(address_, grpc::InsecureServerCredentials());
            builder.RegisterService(&service_);
            cq_ = builder.AddCompletionQueue();
            server_ = builder.BuildAndStart();
            std::cout << "async echo server listening on " << address_ << std::endl;
        }

        void DoRun()
        {
            new RequestHandler(&service_, cq_.get());

            void *tag;
            bool ok;
            while (true)
            {
                GPR_ASSERT(cq_->Next(&tag, &ok));
                GPR_ASSERT(ok);
                static_cast<RequestHandler *>(tag)->Handle();
            }
        }

    private:
        const std::string address_;
        std::unique_ptr<grpc::Server> server_;
        std::unique_ptr<grpc::ServerCompletionQueue> cq_;
        gcppg::echo::Echo::AsyncService service_;
    };
} // anonymous namespace

namespace GCPPG
{
    int mainEchoServerAsync(int argc, char** argv)
    {
        std::string serverAddress = absl::StrFormat("0.0.0.0:%d", FLAGS_echo_server_async_port);
        Server server(serverAddress);
        server.Run();
        return 0;
    }
} // namespace GCPPG
