find_package(GTest REQUIRED)

add_test_executable(MemoryLibTests
    arenaTests.cpp    
    arenaMemoryBlockTests.cpp
)

depend_and_link(MemoryLibTests
    MemoryLib
    CommonTestLib
    GTest::gmock_main
    GTest::gtest_main
)
