cmake_minimum_required(VERSION 3.12)
project(example_print_tables)

add_executable(${PROJECT_NAME} print_tables.cpp)
target_link_libraries(${PROJECT_NAME} fifi)
