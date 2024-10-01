

# 全局搜索grpc依赖包
find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(c-ares CONFIG)

set(SOURCES
    main.cpp
    rpc_manager.cpp
)

# 添加静态库、头文件路径
link_directories(/home/work/proto/build)
include_directories(/home/work/proto/build)

add_executable(server ${SOURCES})

target_link_libraries(server
    PUBLIC
    monitor_proto
    gRPC::grpc
    gRPC::grpc++
)




