find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

add_test_executable(PointerTest
    uniquePtrTests.cpp
    ${Boost_INCLUDE_DIRS}
)

depend_and_link(PointerTest
    PointerLib
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)
