cmake_minimum_required(VERSION 3.10)
project(MoonContainers)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(${CMAKE_CURRENT_SOURCE_DIR}/CMakeFunctions.cmake)

add_subdirectory(stack)
