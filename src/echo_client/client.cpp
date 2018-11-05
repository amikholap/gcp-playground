#include <iostream>
#include <random>
#include <string>

#include "absl/strings/str_format.h"
#include <gflags/gflags.h>
#include <grpc/support/log.h>
#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>

#include "../echo_server/echo.grpc.pb.h"
#include "client.h"

DEFINE_string(echo_client_server_address, "", "");

namespace
{
    class EchoClient
    {
    public:
        EchoClient(std::shared_ptr<grpc::Channel> channel)
            : stub(gcppg::echo::Echo::NewStub(channel))
        {
        }

        gcppg::echo::EchoResponse SendRandom(size_t messageLength)
        {
            grpc::ClientContext context;
            gcppg::echo::EchoResponse response;

            gcppg::echo::EchoRequest request;
            request.set_message(genRandomString(messageLength));

            grpc::Status status = stub->Send(&context, request, &response);
            if (!status.ok())
            {
                throw std::runtime_error(absl::StrFormat(
                    "grpc failed with code %d: %s", status.error_code(), status.error_message()
                ));
            }

            return response;
        }
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

    private:
        std::unique_ptr<gcppg::echo::Echo::Stub> stub;
    };
} // anonymous namespace

namespace GCPPG
{
    int mainEchoClient(int argc, char** argv)
    {
        EchoClient client(
            grpc::CreateChannel(FLAGS_echo_client_server_address, grpc::InsecureChannelCredentials())
        );

        while (true) {
            try
            {
                const auto& response = client.SendRandom(32);
                std::cout << response.message() << std::endl;
            } catch(const std::exception& e)
            {
                std::cerr << e.what() << std::endl;
            }
        }

        return 0;
    }
} // namespace GCPPG
