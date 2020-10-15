# SETUP
cmake_minimum_required(VERSION 3.12)
project(mygit)

# BUILD OPTIONS
set(CMAKE_CXX_STANDARD 17)

include_directories(src/)

#List of source files
set(SRC_FILES
        # Globals
        src/utils/globals.cc

        # Commands
        src/commands/init.cc
        src/commands/add.cc
        src/commands/status.cc
        src/commands/ls-files.cc
        src/commands/hash-object.cc
        src/commands/cat_file.cc
        src/commands/write_tree.cc
        src/commands/commit.cc
        src/commands/diff.cc
        src/commands/log.cc
        src/commands/branch.cc
        src/commands/checkout.cc

        # Utils
        src/utils/files.cc
        src/utils/wrappers.cc
        src/utils/zlib.cc
        src/utils/sha1.cc
        src/utils/options.cc

        # Objects
        src/objects/blob.cc
        src/objects/object_type.cc
        src/objects/tree.cc
        src/objects/commit.cc)

# Compilation flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -pedantic -std=c++17")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -march=native")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3")

# Executables
add_executable(mygit src/main.cc ${SRC_FILES})
add_executable(add_test src/tests/main_add_test.cc ${SRC_FILES})
add_executable(status_test src/tests/main_status_test.cc ${SRC_FILES})


# Linked libraries targetting
target_link_libraries(mygit crypto z)
target_link_libraries(add_test crypto z)
target_link_libraries(status_test crypto z)

# Launch testsuite
add_custom_target(test COMMAND cd ../tests && ./testsuite.sh && cd - > /dev/null)
add_custom_target(tests COMMAND cd ../tests && ./testsuite.sh && cd - > /dev/null)