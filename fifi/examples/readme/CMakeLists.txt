cmake_minimum_required(VERSION 3.12)
project(example_readme)

add_executable(${PROJECT_NAME} readme.cpp)
target_link_libraries(${PROJECT_NAME} fifi)
