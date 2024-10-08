cmake_minimum_required(VERSION 3.10)
project(linux_monitor LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(c-ares CONFIG)

# add_subdirectory(proto)
# add_subdirectory(rpc_manager/client)
# add_subdirectory(rpc_manager/server)
# add_subdirectory(test_monitor)
add_subdirectory(display_monitor)