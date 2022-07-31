include_directories(${Coding_SOURCE_DIR}/kodr/include "~/.local/include")
link_directories("~/.local/lib")

add_library(kodr boneh.cpp data.cpp decoder.cpp encoder.cpp decoder_state.cpp matrix.cpp recoder.cpp)
link_libraries(kodr  "mcl")