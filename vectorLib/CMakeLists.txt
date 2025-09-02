add_static_library(VectorLib
    vector.cpp
    vectorIterator.cpp
)

depend_and_link(VectorLib
    AllocatorLib
)

add_subdirectory(test)
add_subdirectory(perfTest)
