#include <iostream>

#include <google/protobuf/compiler/code_generator.h>
#include <gflags/gflags.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>

#include "absl/strings/str_format.h"
#include "echo.grpc.pb.h"
#include "server.h"

DEFINE_uint32(echo_server_port, 0, "");


namespace gcppg::echo {

    class EchoImpl final : public Echo::Service
    {
        grpc::Status Send(grpc::ServerContext* context, const EchoRequest* request, EchoResponse* response) override
        {
            response->set_message(request->message());
            return grpc::Status::OK;
        }
    };

} // namespace echo

namespace GCPPG
{
    int mainEchoServer(int argc, char** argv)
    {
        std::string serverAddress = absl::StrFormat("0.0.0.0:%d", FLAGS_echo_server_port);

        grpc::reflection::InitProtoReflectionServerBuilderPlugin();

        gcppg::echo::EchoImpl service;

        grpc::ServerBuilder builder;
        builder.AddListeningPort(serverAddress, grpc::InsecureServerCredentials());
        builder.RegisterService(&service);
        std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

        std::cout << "echo server listening on " << serverAddress << std::endl;

        server->Wait();

        return 0;
    }
} // namespace GCPPG
