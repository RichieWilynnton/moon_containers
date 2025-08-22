find_package(Boost 1.88.0 REQUIRED)  

add_static_library(AllocatorLib
    heapAllocator.cpp
    debugAllocator.cpp
    managedSharedMemorySegmentAllocator.cpp
)

depend_and_link(AllocatorLib
    CommonLib
    Boost::boost
)

add_subdirectory(test)
# add_subdirectory(testApp)
