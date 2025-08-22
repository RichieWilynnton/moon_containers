add_test_executable(AllocatorTest
    managedSharedMemorySegmentAllocatorTests.cpp
)

depend_and_link(AllocatorTest
    AllocatorLib
    CommonTestLib
)
