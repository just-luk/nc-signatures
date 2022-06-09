cmake_minimum_required(VERSION 3.12)
project(example_custom_prime)

add_executable(${PROJECT_NAME} custom_prime.cpp)
target_link_libraries(${PROJECT_NAME} fifi)
