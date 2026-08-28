cmake_minimum_required(VERSION 4.3)
project(map_reduce)

set(CMAKE_CXX_STANDARD 23)

find_package(GTest REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(rapidcheck_gtest REQUIRED rapidcheck_gtest)
enable_testing()

add_executable(map_reduce
        main.cpp
        word_count.cpp
)

add_executable(map_reduce_tests
        test.cpp
        word_count.cpp
)
target_link_libraries(map_reduce_tests PRIVATE GTest::GTest GTest::Main rapidcheck)
target_compile_options(map_reduce_tests PRIVATE -fsanitize=address)
target_link_options(map_reduce_tests PRIVATE -fsanitize=address)

add_test(NAME map_reduce_tests COMMAND map_reduce_tests)