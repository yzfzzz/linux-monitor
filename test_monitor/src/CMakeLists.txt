find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(c-ares CONFIG)
set(SOURCES
   main.cpp
   monitor/cpu_softirq_monitor.cpp
   monitor/cpu_load_monitor.cpp
   monitor/cpu_stat_monitor.cpp
   monitor/mem_monitor.cpp
   monitor/net_monitor.cpp
   utils/read_file.cpp
)

link_directories(/home/work/proto/build /home/work/rpc_manager/client/build)
# include_directories(/work/rpc_manager)

add_executable(monitor ${SOURCES})

# target_include_directories(monitor PUBLIC
#   ../include/..
# )
target_include_directories(monitor PUBLIC
 ${CMAKE_CURRENT_SOURCE_DIR}/../include
  /home/work/rpc_manager
  /home/work/proto/build
)

target_link_libraries(monitor
    PUBLIC
    monitor_proto
    client
    boost_system
    boost_chrono
    gRPC::grpc
    gRPC::grpc++
)