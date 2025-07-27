find_package(GTest REQUIRED)

add_test_executable(MathTest
    mathTests.cpp
)

depend_and_link(MathTest
    CommonLib 
    GTest::gmock_main
    GTest::gtest_main
)
