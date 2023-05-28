# CMake generated Testfile for 
# Source directory: C:/Users/pawel/Desktop/Tietoevry/player/tests
# Build directory: C:/Users/pawel/Desktop/Tietoevry/player/out/build/x64-Debug/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(runUnitTests "C:/Users/pawel/Desktop/Tietoevry/player/out/build/x64-Debug/tests/player_tests.exe")
set_tests_properties(runUnitTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/pawel/Desktop/Tietoevry/player/tests/CMakeLists.txt;40;add_test;C:/Users/pawel/Desktop/Tietoevry/player/tests/CMakeLists.txt;0;")
subdirs("../_deps/googletest-build")
