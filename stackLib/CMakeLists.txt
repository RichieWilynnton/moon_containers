add_static_library(StackLib
    stack.cpp 
)
depend_and_link(StackLib
    PointerLib)

add_subdirectory(test)
