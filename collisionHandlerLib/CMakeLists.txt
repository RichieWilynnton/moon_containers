add_static_library(CollisionHandlerLib
    openAddressingCollisionHandler.cpp
    openAddressingCollisionHandlerIterator.cpp
)

depend_and_link(CollisionHandlerLib
    AllocatorLib
)

