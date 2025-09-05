add_static_library(MemoryLib
    arena.cpp
    arenaChunk.cpp
    arenaChunkHeader.cpp
    arenaMemoryBlock.cpp
)

depend_and_link(MemoryLib
    CommonLib
)

add_subdirectory(test)
