set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Qt5
    REQUIRED COMPONENTS
    Core
    Widgets
    Charts
    )

find_package(Threads REQUIRED)

set(SOURCES
    main.cpp
    monitor_inter.cpp
    cpu_softirq_model.cpp
    cpu_load_model.cpp
    monitor_widget.cpp
    cpu_stat_model.cpp
    mem_model.cpp
    net_model.cpp

    mem_piechart.cpp
    cpu_load_barchart.cpp
    cpu_stat_barchart.cpp
)
link_directories(${PROJECT_SOURCE_DIR}/build/rpc_manager/client ${PROJECT_SOURCE_DIR}/build/proto)

add_executable(display ${SOURCES})

set_target_properties(display PROPERTIES AUTOMOC TRUE)

target_include_directories(display PUBLIC
    ${PROJECT_SOURCE_DIR}/build/proto
    ${PROJECT_SOURCE_DIR}/rpc_manager
    ${PROTOBUF_INCLUDE_DIRS} 
)

target_link_libraries(display
    PUBLIC
    client
    monitor_proto
    Qt5::Widgets
    Qt5::Core
    Qt5::Charts
    Threads::Threads
    # qwt-qt5
)