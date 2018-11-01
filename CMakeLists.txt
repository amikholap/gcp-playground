cmake_minimum_required(VERSION 3.10)
project(gcp_playground)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")
#set(CMAKE_VERBOSE_MAKEFILE on)

set(CONTRIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/contrib")

set(gRPC_PROTOBUF_PROVIDER "package" CACHE STRING "Provider of protobuf library")
option(protobuf_BUILD_TESTS "Build tests" OFF)

add_subdirectory(contrib)
find_package(abseil REQUIRED)
find_package(gflags REQUIRED)
find_package(grpc REQUIRED)
find_package(Protobuf REQUIRED)

add_executable(gcppg main.cpp echo_server/echo.grpc.pb.cc echo_server/echo.pb.cc echo_server/server.cpp)
target_compile_options(gcppg PRIVATE -Werror)
target_link_libraries(gcppg grpc++_reflection grpc++ ${PROTOBUF_LIBRARIES} standalone_absl gflags)
