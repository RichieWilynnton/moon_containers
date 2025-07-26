add_static_library(AllocatorLib
    heapAllocator.cpp
)

depend_and_link(AllocatorLib
    CommonLib
)

add_subdirectory(test)
