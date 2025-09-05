#pragma once

#include <MemoryLib/arenaChunk.hpp>
#include <cstddef>

namespace Moon
{

void ArenaChunk::Reset() {}

void* ArenaChunk::GetData()
{
    return mStart;
}

size_t ArenaChunk::GetCapacity()
{
    return mCapacity;
}

}  // namespace Moon
