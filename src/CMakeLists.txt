set(CONTRIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/contrib")

set(gRPC_PROTOBUF_PROVIDER "package" CACHE STRING "Provider of protobuf library")
option(protobuf_BUILD_TESTS "Build tests" OFF)

add_subdirectory(contrib)
find_package(abseil REQUIRED)
find_package(gflags REQUIRED)
find_package(grpc REQUIRED)
find_package(Protobuf REQUIRED)

add_executable(
        gcppg
        main.cpp
        echo_server/echo.grpc.pb.cc
        echo_server/echo.pb.cc
        echo_server/server.cpp
        echo_server/server_async.cpp
        echo_client/client.cpp
)
target_compile_options(gcppg PRIVATE -Werror)
target_link_libraries(gcppg grpc++_reflection grpc++ ${PROTOBUF_LIBRARIES} standalone_absl gflags)
