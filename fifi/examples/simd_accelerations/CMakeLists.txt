cmake_minimum_required(VERSION 3.12)
project(example_simd_accelerations)

add_executable(${PROJECT_NAME} simd_accelerations.cpp)
target_link_libraries(${PROJECT_NAME} fifi)
