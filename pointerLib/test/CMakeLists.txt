find_package(GTest REQUIRED)

add_test_executable(PointerTest
    uniquePtrTests.cpp
)

depend_and_link(PointerTest
    PointerLib
    CommonTestLib
    GTest::gmock_main
    GTest::gtest_main
)
