find_package(GTest REQUIRED)
add_test_executable(VectorTest
    vectorTests.cpp
)

depend_and_link(VectorTest
    VectorLib
    CommonTestLib
    GTest::gmock_main
    GTest::gtest_main
)
