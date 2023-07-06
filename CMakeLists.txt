cmake_minimum_required(VERSION 3.14)
project(hw04)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)
add_compile_options(-Wall -Wextra -Wpedantic -Werror -g)
#add_library(malloc_1 malloc_1.cpp)

add_library(malloc_2 malloc_2.cpp)

#add_executable(main main.cpp)
#target_link_libraries(main malloc_2)

add_executable(test_part2 test/part2/test_part2.cpp)
target_link_libraries(test_part2 malloc_2 GTest::gtest_main)
gtest_discover_tests(test_part2)


