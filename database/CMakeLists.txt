find_package(Threads REQUIRED)
link_directories(${PROJECT_SOURCE_DIR}/database/lib)

set(DATABASE_SRC
    src/main.cpp
    src/ConnectionPool.cpp
    src/MysqlConn.cpp
)

include_directories(${PROJECT_SOURCE_DIR}/database/include)

add_library(database ${DATABASE_SRC})

target_link_libraries(database
  PUBLIC
  Threads::Threads
  mysqlclient
)