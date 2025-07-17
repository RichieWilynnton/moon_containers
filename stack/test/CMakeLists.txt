find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

add_test_executable(StackTest
    stackTests.cpp
    ${Boost_INCLUDE_DIRS}
)

depend_and_link(StackTest
    StackLib
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)
