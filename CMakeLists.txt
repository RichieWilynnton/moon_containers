cmake_minimum_required(VERSION 3.10)
project(MoonContainers)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeFunctions.cmake)

option(RUN_TESTS_AFTER_BUILD "Run tests automatically after building" ON)

# Initialize list of all test targets
set(ALL_TEST_TARGETS "" CACHE INTERNAL "List of all test targets")

add_subdirectory(stack)
add_subdirectory(pointer)
# add_subdirectory(arrow)

# Add custom target to run all tests after build
if(RUN_TESTS_AFTER_BUILD)
    add_custom_target(run_all_tests ALL
        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running tests..."
    )
    
    # Make the run_all_tests target depend on all test executables
    foreach(test_target IN LISTS ALL_TEST_TARGETS)
        add_dependencies(run_all_tests ${test_target})
    endforeach()
endif()
