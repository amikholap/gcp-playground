cmake_minimum_required(VERSION 3.10)
project(gcp_playground)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/")
#set(CMAKE_VERBOSE_MAKEFILE on)

add_subdirectory(src)
