add_static_library(PointerLib
    uniquePtr.cpp
    sharedPtr.cpp
    weakPtr.cpp
    controlBlock.cpp
)

add_subdirectory(test)
