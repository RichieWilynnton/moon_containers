add_static_library(MapTest
    hashMapTests.cpp)

depend_and_link(MapTest
    MapLib
    CommonTestLib
    AllocatorLib
)
