# CMake generated Testfile for 
# Source directory: /workspace/just-luk.github.io/kodo
# Build directory: /workspace/just-luk.github.io/kodo/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(kodo_test "kodo_test")
set_tests_properties(kodo_test PROPERTIES  _BACKTRACE_TRIPLES "/workspace/just-luk.github.io/kodo/CMakeLists.txt;124;add_test;/workspace/just-luk.github.io/kodo/CMakeLists.txt;0;")
subdirs("endian")
subdirs("ranbo")
subdirs("bourne")
subdirs("recycle")
subdirs("fifi")
subdirs("resolve_symlinks/gtest-source")
subdirs("resolve_symlinks/stub")
