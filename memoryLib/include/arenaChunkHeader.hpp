#pragma once

#include <MemoryLib/arenaChunk.hpp>

namespace Moon
{

// Putting ChunkHeader beside the actual chunk allows for O(1) release
class ArenaChunkHeader : public ArenaChunk
{
   public:
    ArenaChunkHeader* mNext;
    ArenaChunkHeader* mPrev;
    bool mIsUsed;

    ArenaChunkHeader(std::byte* chunkPtr, const size_t chunkSize)
        : ArenaChunk(chunkPtr, chunkSize), mNext(nullptr), mPrev(nullptr), mIsUsed(false)
    {
    }
};
}  // namespace Moon
