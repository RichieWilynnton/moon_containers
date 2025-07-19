find_package(GTest REQUIRED)

add_test_executable(StackTest
    stackTests.cpp
)

depend_and_link(StackTest
    StackLib
    CommonTestLib
    GTest::gmock_main
    GTest::gtest_main
)
