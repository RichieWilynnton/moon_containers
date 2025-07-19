find_package(GTest REQUIRED)

add_static_library(CommonTestLib
    dummy.cpp
    dummyTracker.cpp)

depend_and_link(CommonTestLib
    GTest::gmock_main
    GTest::gtest_main
)
