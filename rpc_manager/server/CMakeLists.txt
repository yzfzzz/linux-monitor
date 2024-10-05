set(SOURCES
    main.cpp
    rpc_manager.cpp
)

# 添加静态库、头文件路径
link_directories(${PROJECT_SOURCE_DIR}/build/proto)
include_directories(${PROJECT_SOURCE_DIR}/build/proto)

add_executable(server ${SOURCES})

target_link_libraries(server
    PUBLIC
    monitor_proto
)




