set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/mprpc/lib)
include_directories(${PROJECT_SOURCE_DIR}/mprpc/src/include)
link_directories(${PROJECT_SOURCE_DIR}/mprpc/lib)
add_subdirectory(src)