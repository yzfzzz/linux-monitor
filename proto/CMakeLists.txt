# 找到全局环境中的grpc依赖包
#[[
指定使用 config 模式来查找 protobuf 库。在 config 模式下，CMake 会搜索名为 <package_name>-config.cmake 
或 config.cmake 的文件（对于 protobuf 库来说，就是 protobuf-config.cmake 文件），这些文件通常由库的安装
过程生成，包含了库的头文件路径、库文件路径等信息
]]

# 为set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)



find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(c-ares CONFIG)


set(PROTO_FILES
    monitor_info.proto
    cpu_load.proto
    cpu_softirq.proto
    cpu_stat.proto
    mem_info.proto
    net_info.proto
)

# 制作静态库
add_library(monitor_proto ${PROTO_FILES})
# 链接静态库
target_link_libraries(monitor_proto
    PUBLIC
        protobuf::libprotobuf
        gRPC::grpc
        gRPC::grpc++
)
# 指定库的头文件路径
target_include_directories(monitor_proto PUBLIC
${PROTOBUF_INCLUDE_DIRS}

# 这个感觉可以不加
${CMAKE_CURRENT_BINARY_DIR}
)

get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
protobuf_generate(TARGET monitor_proto LANGUAGE cpp)
protobuf_generate(TARGET monitor_proto LANGUAGE grpc GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}")

