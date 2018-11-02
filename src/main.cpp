#include <cstring>
#include <iostream>

#include <gflags/gflags.h>

#include "echo_client/client.h"
#include "echo_server/server.h"


using MainFunc = int (*)(int, char**);

std::pair<const char*, MainFunc> applications[] = {
        {"echo_client", GCPPG::mainEchoClient},
        {"echo_server", GCPPG::mainEchoServer}
};


int printHelp()
{
    std::cerr << "Available commands:" << std::endl;
    for (auto& application : applications)
        std::cerr << "gcppg " << application.first << " [args] " << std::endl;
    return -1;
}


int main(int argc, char** argv)
{
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    if (argc < 2) {
        return printHelp();
    }

    MainFunc mainFunc = [] (int, char**) { return printHelp(); };
    const char* cmdApp = argv[1];
    for (auto& application : applications) {
        if (strcmp(cmdApp, application.first) == 0)
        {
            mainFunc = application.second;
        }
    }

    return mainFunc(argc, argv);
}