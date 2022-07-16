cmake_minimum_required(VERSION 3.10)

# set the project name
project(Coding)

include_directories(${Coding_SOURCE_DIR}/kodr/include "~/.local/include")
link_directories("~/.local/lib")

#
add_subdirectory(kodr)

# add the executable
add_executable(coding.exe coding.cpp)
target_link_libraries(coding.exe kodr mcl)
target_compile_options(coding.exe PRIVATE -O3)