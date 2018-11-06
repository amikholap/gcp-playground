set(CONTRIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/contrib")

set(gRPC_PROTOBUF_PROVIDER "package" CACHE STRING "Provider of protobuf library")
option(protobuf_BUILD_TESTS "Build tests" OFF)

add_subdirectory(contrib)
find_package(abseil REQUIRED)
find_package(gflags REQUIRED)
find_package(grpc REQUIRED)
find_package(Protobuf REQUIRED)
find_package(TBB REQUIRED)

add_executable(
        gcppg
        main.cpp
        echo_server/echo.grpc.pb.cc
        echo_server/echo.pb.cc
        echo_server/server.cpp
        echo_server/server_async.cpp
        echo_client/client.cpp
        echo_client/client_async.cpp
)
target_compile_options(gcppg PRIVATE -Werror)
target_link_libraries(gcppg grpc++_reflection grpc++ ${PROTOBUF_LIBRARIES} standalone_absl ${TBB_LIBRARY} gflags)
target_include_directories(gcppg PUBLIC ${TBB_INCLUDE_DIR})
