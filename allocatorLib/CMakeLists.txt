add_static_library(AllocatorLib
    heapAllocator.cpp
    debugAllocator.cpp
)

depend_and_link(AllocatorLib
    CommonLib
)

add_subdirectory(test)
