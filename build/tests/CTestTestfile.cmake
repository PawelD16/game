# CMake generated Testfile for 
# Source directory: C:/Users/pawel/Desktop/Tietoevry/player/tests
# Build directory: C:/Users/pawel/Desktop/Tietoevry/player/build/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(runUnitTests "C:/Users/pawel/Desktop/Tietoevry/player/build/tests/Debug/player_tests.exe")
  set_tests_properties(runUnitTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/pawel/Desktop/Tietoevry/player/tests/CMakeLists.txt;40;add_test;C:/Users/pawel/Desktop/Tietoevry/player/tests/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(runUnitTests "C:/Users/pawel/Desktop/Tietoevry/player/build/tests/Release/player_tests.exe")
  set_tests_properties(runUnitTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/pawel/Desktop/Tietoevry/player/tests/CMakeLists.txt;40;add_test;C:/Users/pawel/Desktop/Tietoevry/player/tests/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(runUnitTests "C:/Users/pawel/Desktop/Tietoevry/player/build/tests/MinSizeRel/player_tests.exe")
  set_tests_properties(runUnitTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/pawel/Desktop/Tietoevry/player/tests/CMakeLists.txt;40;add_test;C:/Users/pawel/Desktop/Tietoevry/player/tests/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(runUnitTests "C:/Users/pawel/Desktop/Tietoevry/player/build/tests/RelWithDebInfo/player_tests.exe")
  set_tests_properties(runUnitTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/pawel/Desktop/Tietoevry/player/tests/CMakeLists.txt;40;add_test;C:/Users/pawel/Desktop/Tietoevry/player/tests/CMakeLists.txt;0;")
else()
  add_test(runUnitTests NOT_AVAILABLE)
endif()
subdirs("../_deps/googletest-build")
