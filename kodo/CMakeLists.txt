cmake_minimum_required(VERSION 3.12)
project(kodo)

find_package(Python COMPONENTS Interpreter)
# Use waf to resolve dependencies
if(NOT DEFINED STEINWURF_RESOLVE)
  message(STATUS "Resolving dependencies...")
  execute_process(
    COMMAND ${Python_EXECUTABLE} waf resolve ${STEINWURF_RESOLVE_OPTIONS}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE STATUS)
  if(STATUS AND NOT STATUS EQUAL 0)
    message(FATAL_ERROR "Failed: ${STATUS}")
  endif()
  set(STEINWURF_RESOLVE "${CMAKE_CURRENT_SOURCE_DIR}/resolve_symlinks")
  set(STEINWURF_TOP_NAME kodo)
endif()

# endian dependency
if(NOT TARGET steinwurf::endian)
  add_subdirectory("${STEINWURF_RESOLVE}/endian" endian EXCLUDE_FROM_ALL)
endif()

# ranbo dependency
if(NOT TARGET steinwurf::ranbo)
  add_subdirectory("${STEINWURF_RESOLVE}/ranbo" ranbo EXCLUDE_FROM_ALL)
endif()

# bourne dependency
if(NOT TARGET steinwurf::bourne)
  add_subdirectory("${STEINWURF_RESOLVE}/bourne" bourne EXCLUDE_FROM_ALL)
endif()

# recycle dependency
if(NOT TARGET steinwurf::recycle)
  add_subdirectory("${STEINWURF_RESOLVE}/recycle" recycle EXCLUDE_FROM_ALL)
endif()

# fifi dependency
if(NOT TARGET steinwurf::fifi)
  add_subdirectory("${STEINWURF_RESOLVE}/fifi" fifi EXCLUDE_FROM_ALL)
endif()

# Define library
file(GLOB_RECURSE kodo_sources ./src/*.cpp)

# Is this the top-level steinwurf project?
if(${PROJECT_NAME} STREQUAL ${STEINWURF_TOP_NAME})
  # Create static library
  add_library(kodo STATIC ${kodo_sources})
  # Get all steinwurf object libraries and link directly with them.
  get_property(steinwurf_object_libraries GLOBAL
               PROPERTY steinwurf::object_libraries)
  target_link_libraries(kodo PRIVATE ${steinwurf_object_libraries})

  # Install library
  install(FILES $<TARGET_FILE:kodo> DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
else()
  # Create object library
  add_library(kodo OBJECT ${kodo_sources})
  # Add this library to a global list of steinwurf object libraries
  set_property(GLOBAL APPEND PROPERTY steinwurf::object_libraries
                                      steinwurf::kodo)
  # Link object dependencies
  target_link_libraries(kodo PRIVATE steinwurf::bourne)
  target_link_libraries(kodo PRIVATE steinwurf::fifi)
endif()

# Link header only dependencies
target_link_libraries(kodo PRIVATE steinwurf::ranbo)
target_link_libraries(kodo PRIVATE steinwurf::endian)
target_link_libraries(kodo PRIVATE steinwurf::recycle)

target_include_directories(kodo INTERFACE src)
target_compile_features(kodo PUBLIC cxx_std_14)
add_library(steinwurf::kodo ALIAS kodo)

# Install headers excluding "detail" as these are internal to the library.
install(
  DIRECTORY ./src/kodo
  DESTINATION ${CMAKE_INSTALL_PREFIX}/include
  FILES_MATCHING
  PATTERN *.hpp
  PATTERN ./src/kodo/detail EXCLUDE)

# Is top level project?
if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
  # Build executables
  add_executable(hello_world examples/hello_world.cpp)
  target_link_libraries(hello_world kodo)

  add_executable(perpetual_encode_decode_simple
                 examples/perpetual/encode_decode_simple.cpp)
  target_link_libraries(perpetual_encode_decode_simple kodo)

  enable_testing()

  if(CMAKE_C_COMPILER_ID MATCHES "MSVC")
    # For Windows: Prevent overriding the parent project's compiler/linker
    # settings
    set(gtest_force_shared_crt
        ON
        CACHE BOOL "" FORCE)
  endif()

  # Google Test dependency
  add_subdirectory("${STEINWURF_RESOLVE}/gtest-source")

  # Stub Test dependency
  add_subdirectory("${STEINWURF_RESOLVE}/stub")

  # Build test executable
  file(GLOB_RECURSE kodo_test_sources ./test/*.cpp)
  add_executable(kodo_test ${kodo_test_sources})
  target_link_libraries(kodo_test steinwurf::kodo)
  target_link_libraries(kodo_test gtest_main)
  target_link_libraries(kodo_test steinwurf::fifi)
  target_link_libraries(kodo_test steinwurf::recycle)
  target_link_libraries(kodo_test steinwurf::stub)
  target_link_libraries(kodo_test steinwurf::bourne)
  target_link_libraries(kodo_test steinwurf::endian)
  target_link_libraries(kodo_test steinwurf::ranbo)

  add_test(kodo_test kodo_test)

endif()
