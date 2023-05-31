# CMake generated Testfile for 
# Source directory: /home/stud/player/game/tests
# Build directory: /home/stud/player/game/build/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(runUnitTests "/home/stud/player/game/build/tests/player_tests")
set_tests_properties(runUnitTests PROPERTIES  _BACKTRACE_TRIPLES "/home/stud/player/game/tests/CMakeLists.txt;40;add_test;/home/stud/player/game/tests/CMakeLists.txt;0;")
subdirs("../_deps/googletest-build")
