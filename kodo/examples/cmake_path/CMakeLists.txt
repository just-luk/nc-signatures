cmake_minimum_required (VERSION 3.12)
project(cmake_example)

add_subdirectory("../../../kodo" kodo)
add_executable(cmake_example main.cpp)

target_link_libraries(cmake_example steinwurf::kodo)