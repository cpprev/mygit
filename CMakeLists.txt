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
        src/commands/add.cc
        src/commands/status.cc
        src/commands/ls-files.cc
        src/commands/hash-object.cc
        src/commands/cat_file.cc

        # Utils
        src/utils/files.cc
        src/utils/wrappers.cc
        src/utils/zlib.cc

        # Objects
        src/objects/blob.cc
        src/objects/object_type.cc)

# Compilation flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -pedantic -std=c++17")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -march=native")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3")

add_executable(mygit src/main.cc ${SRC_FILES})

add_executable(hash src/mainHash.cc)

add_executable(compress src/mainCompress.cc)

target_link_libraries(mygit crypto z)
target_link_libraries(hash crypto)
target_link_libraries(compress z)