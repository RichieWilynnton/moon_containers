find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)

add_test_executable(PointerTests
    uniquePtrTests.cpp
)

depend_and_link(PointerTests
    PointerLib
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

include_directories(${Boost_INCLUDE_DIRS})
