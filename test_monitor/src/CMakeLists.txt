set(SOURCES
   main.cpp
   monitor/cpu_softirq_monitor.cpp
   monitor/cpu_load_monitor.cpp
   monitor/cpu_stat_monitor.cpp
   monitor/mem_monitor.cpp
   monitor/net_monitor.cpp
   utils/read_file.cpp
)

link_directories(${PROJECT_SOURCE_DIR}/build/proto ${PROJECT_SOURCE_DIR}/build/rpc_manager/client)

add_executable(monitor ${SOURCES})

target_include_directories(monitor PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/../include
  ${PROJECT_SOURCE_DIR}/rpc_manager
  ${PROJECT_SOURCE_DIR}/build/proto
)

target_link_libraries(monitor
    PUBLIC
    monitor_proto
    client
    boost_system
    boost_chrono
)