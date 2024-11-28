set(SOURCES
    main.cpp
    rpc_manager.cpp
)

# 添加静态库、头文件路径
# 这里的proto静态库路径不太好，后续改一下
link_directories(${PROJECT_SOURCE_DIR}/build/proto ${PROJECT_SOURCE_DIR}/mprpc/lib ${PROJECT_SOURCE_DIR}/database/lib)
include_directories(${PROJECT_SOURCE_DIR}/build/proto ${PROJECT_SOURCE_DIR}/tool ${PROJECT_SOURCE_DIR}/mprpc/src/include ${PROJECT_SOURCE_DIR}/database/include)

add_executable(server ${SOURCES})

target_link_libraries(server
    PUBLIC
    mprpc
    monitor_proto
    database
)




