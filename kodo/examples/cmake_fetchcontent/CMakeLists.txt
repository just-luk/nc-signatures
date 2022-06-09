cmake_minimum_required (VERSION 3.12)
project(cmake_example)

include(FetchContent)

FetchContent_Declare(
    kodo
    GIT_REPOSITORY git@github.com:steinwurf/kodo.git
    GIT_TAG        8.1.0
)
FetchContent_MakeAvailable(kodo)

add_executable(cmake_example main.cpp)

target_link_libraries(cmake_example steinwurf::kodo)