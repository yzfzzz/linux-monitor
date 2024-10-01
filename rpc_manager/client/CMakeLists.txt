set(SOURCES rpc_client.cpp)

link_directories(/home/work/proto/build)
include_directories(/home/work/proto/build)


add_library(client ${SOURCES})

target_link_libraries(client 
    PUBLIC 
    monitor_proto)
