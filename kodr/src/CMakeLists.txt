include_directories(${Coding_SOURCE_DIR}/kodr/include "~/.local/include")
link_directories("~/.local/lib")

add_library(kodr boneh.cpp chang.cpp data.cpp catalano.cpp decoder.cpp encoder.cpp decoder_state.cpp li.cpp matrix.cpp recoder.cpp zhang.cpp)
link_libraries(kodr  "mcl")