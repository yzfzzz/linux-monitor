set(CMAKE_CXX_STANDARD 17)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Qt5
    REQUIRED COMPONENTS
    Core
    Widgets
    )

find_package(Threads REQUIRED)
find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(c-ares CONFIG)

set(SOURCES
    main.cpp
    monitor_inter.cpp
    cpu_softirq_model.cpp
    cpu_load_model.cpp
    monitor_widget.cpp
    cpu_stat_model.cpp
    mem_model.cpp
    net_model.cpp
)

link_directories(/home/work/rpc_manager/client/build /home/work/proto/build)

add_executable(display ${SOURCES})

set_target_properties(display PROPERTIES AUTOMOC TRUE)

target_include_directories(display PUBLIC
    /home/work/proto/build
    /home/work/rpc_manager
    ${PROTOBUF_INCLUDE_DIRS} 
)

target_link_libraries(display
    PUBLIC
    client
    monitor_proto
    Qt5::Widgets
    Qt5::Core
    Threads::Threads

    protobuf::libprotobuf
    gRPC::grpc
    gRPC::grpc++
    # qwt-qt5
)