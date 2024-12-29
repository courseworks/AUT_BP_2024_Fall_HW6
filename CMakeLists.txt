cmake_minimum_required(VERSION 3.18)
project(AUT_BP_2024_Fall_HW6)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set C standard
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)


find_package(GTest REQUIRED)


include_directories(include/)

add_executable(main
        src/main.cpp
        src/Q1_Trie.c
        src/unit_test.cpp
)

# Set compiler flags for C++ and c.
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wpedantic")
set(CMAKE_C_FLAGS "-Wall -Wextra -Werror -Wpedantic")


# Link the executable to the Google Test libraries.
target_link_libraries(main
        GTest::GTest
        GTest::Main
)
