find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

add_test_executable(TestUtilTest
    callTrackerTests.cpp
    ${Boost_INCLUDE_DIRS}
)

depend_and_link(TestUtilTest
    TestUtilLib
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)
