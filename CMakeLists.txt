cmake_minimum_required(VERSION 3.10)
project(linux_monitor LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

# 生成debug版本, 可以进行gdb调试
# set(CMAKE_BUILD_TYPE "Debug")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(proto)
add_subdirectory(mprpc)
add_subdirectory(rpc_manager/server)
add_subdirectory(rpc_manager/client)
add_subdirectory(test_monitor)
add_subdirectory(display_monitor)