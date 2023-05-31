cmake_minimum_required(VERSION 3.12)
project(tests)
add_executable(player_tests "player_tests.cpp")

link_directories("../")

###########################
# Add Google Test
###########################

include(FetchContent)

# cmake_policy(SET CMP0135 NEW)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)


# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# Now simply link against gtest or gtest_main as needed. Eg
# add_executable(example example.cpp)
# target_link_libraries(player gtest_main)
# add_test(NAME example_test COMMAND player)


# Link the Google Test library
target_link_libraries(player_tests gtest gtest_main)
target_link_libraries(player_tests gmock gmock_main)
target_link_libraries(player_tests player_lib)

# Add a test target
enable_testing()

add_test(NAME runUnitTests COMMAND player_tests)