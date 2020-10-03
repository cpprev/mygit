# SETUP
cmake_minimum_required(VERSION 3.12)
project(mygit)

# BUILD OPTIONS
set(CMAKE_CXX_STANDARD 17)

include_directories(src/)

#List of source files
set(SRC_FILES
        # Commands
        src/commands/init.cc

        # Utils
        src/utils/files.cc
        src/utils/wrappers.cc)

# Compilation flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -pedantic -std=c++17")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -march=native")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3")

add_executable(mygit src/main.cc ${SRC_FILES})

add_executable(hash src/mainHash.cc)

target_link_libraries(mygit crypto)
target_link_libraries(hash crypto)