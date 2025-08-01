# CMake generated Testfile for 
# Source directory: /Users/shibu/CLionProjects/gameboy
# Build directory: /Users/shibu/CLionProjects/gameboy/cmake-build-debug
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(check_gbe "/Users/shibu/CLionProjects/gameboy/cmake-build-debug/tests/check_gbe")
set_tests_properties(check_gbe PROPERTIES  _BACKTRACE_TRIPLES "/Users/shibu/CLionProjects/gameboy/CMakeLists.txt;99;add_test;/Users/shibu/CLionProjects/gameboy/CMakeLists.txt;0;")
subdirs("lib")
subdirs("gbemu")
subdirs("tests")
