cmake_minimum_required(VERSION 3.12)
project("mediator")
add_executable(mediator "main.cpp")

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
cmake_policy(SET CMP0141 NEW)
set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()


# Add a source field for each class
set(SOURCES
	FileValidator.cpp
    Mediator.cpp
    utils.cpp
)

# Add the header files for each class
set(HEADERS
    FileValidator.h
    Mediator.h
    utils.h
    consts.h
)

# Include the header files directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Add the source and header files to the executable
target_sources(mediator PRIVATE ${SOURCES} ${HEADERS})

