# Find CMakeLists.txt files in subdirectories and add them as subdirectories
file(GLOB_RECURSE example_configs "**/CMakeLists.txt")
foreach(example_config ${example_configs})
    # Remove "/CMakeLists.txt" from the path and add the path to directory as subdirectory
    string(REPLACE "CMakeLists.txt" "" example_config_directory ${example_config})
    add_subdirectory(${example_config_directory})
endforeach()
