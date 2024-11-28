cmake_minimum_required(VERSION 3.10)
project(test_database LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
find_package(Threads REQUIRED)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/)
set(SOURCES
    main.cpp
    ${PROJECT_SOURCE_DIR}/../../database/src/ConnectionPool.cpp
    ${PROJECT_SOURCE_DIR}/../../database/src/MysqlConn.cpp
)



add_executable(test_mysql ${SOURCES})

target_include_directories(test_mysql PUBLIC
  ${PROJECT_SOURCE_DIR}/../../database/include
  ${PROJECT_SOURCE_DIR}/../../tool
)

target_link_libraries(test_mysql 
  PUBLIC
  Threads::Threads
  mysqlclient
)