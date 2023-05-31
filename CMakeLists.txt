cmake_minimum_required(VERSION 3.12)
project("player")
add_executable(player "main.cpp")

# Set C++ standard to C++17
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED True)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
cmake_policy(SET CMP0141 NEW)
set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()


# Add a source field for each class
set(SOURCES
	Base.cpp
	Catapult.cpp
	Knight.cpp
	Pikeman.cpp
	Player.cpp
	Ram.cpp
	Swordsman.cpp
	AttackMoveUnit.cpp
	Archer.cpp
	Worker.cpp
	IAttackable.cpp
)

# Add the header files for each class
set(HEADERS
	Base.h
	Catapult.h
	Knight.h
	Pikeman.h
	Player.h
	Ram.h
	Swordsman.h
	AttackMoveUnit.h
	Archer.h
	Worker.h
	IAttackable.h
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Add the source and header files to the executable
target_sources(player PRIVATE ${SOURCES} ${HEADERS})

add_library(player_lib ${SOURCES} ${HEADERS})

# Include the Google Test directory
add_subdirectory(tests)

add_test(NAME runUnitTests COMMAND player_tests)