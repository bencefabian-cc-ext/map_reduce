cmake_minimum_required(VERSION 3.30)
project(map_reduce)

set(CMAKE_CXX_STANDARD 23)

find_package(GTest REQUIRED)

# rapidcheck (with gtest integration): prefer CMake config, fall back to pkg-config.
find_package(rapidcheck CONFIG QUIET)
if(TARGET rapidcheck_gtest)
       message(STATUS "rapidcheck: using find_package")
       add_library(project_rapidcheck INTERFACE)
       target_link_libraries(project_rapidcheck INTERFACE rapidcheck rapidcheck_gtest)
else()
       message(STATUS "rapidcheck: falling back to pkg-config")
       find_package(PkgConfig REQUIRED)
       pkg_check_modules(rapidcheck_gtest REQUIRED IMPORTED_TARGET rapidcheck_gtest)
       add_library(project_rapidcheck INTERFACE)
       target_link_libraries(project_rapidcheck INTERFACE PkgConfig::rapidcheck_gtest)
endif()

enable_testing()

add_executable(map_reduce
        main.cpp
        word_count.cpp
)

add_executable(map_reduce_tests
        test.cpp
        word_count.cpp
)
target_link_libraries(map_reduce_tests PRIVATE
        GTest::gtest GTest::gtest_main
        project_rapidcheck
)
target_compile_options(map_reduce_tests PRIVATE -fsanitize=address)
target_link_options(map_reduce_tests PRIVATE -fsanitize=address)

add_test(NAME map_reduce_tests COMMAND map_reduce_tests)
