cmake_minimum_required(VERSION 3.10)
project(epidemic_sim)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/graph.cpp
    src/simulator.cpp
    src/analysis.cpp
    src/cli.cpp
    src/main.cpp
)

# Create executable
add_executable(epidemic_sim ${SOURCES})

# Optional: Add warning flags for better code quality
if(MSVC)
    target_compile_options(epidemic_sim PRIVATE /W4)
else()
    target_compile_options(epidemic_sim PRIVATE -Wall -Wextra -Wpedantic)
endif()
