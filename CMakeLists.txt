cmake_minimum_required(VERSION 3.10)
project(MoonContainers)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeFunctions.cmake)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)  
endif()

set(BENCHMARK_ENABLE_TESTING OFF)  # Turns off Benchmark's internal tests
set(BENCHMARK_ENABLE_GTEST_TESTS OFF)  # Turns off GTest dependency for Benchmark

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)
FetchContent_Declare(
    googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.9.4
)
FetchContent_MakeAvailable(googletest)
FetchContent_MakeAvailable(googlebenchmark)

enable_testing()
option(RUN_TESTS_AFTER_BUILD "Run tests automatically after building" ON)
set(ALL_TEST_TARGETS "" CACHE INTERNAL "List of all test targets")

add_subdirectory(stackLib)
add_subdirectory(pointerLib)
add_subdirectory(vectorLib)
add_subdirectory(commonLib)
add_subdirectory(allocatorLib)
add_subdirectory(memoryLib)
# add_subdirectory(collisionHandlerLib)
# add_subdirectory(mapLib)

if(RUN_TESTS_AFTER_BUILD)
    add_custom_target(run_all_tests ALL
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running tests..."
    )
    
    foreach(test_target IN LISTS ALL_TEST_TARGETS)
        add_dependencies(run_all_tests ${test_target})
    endforeach()
endif()
