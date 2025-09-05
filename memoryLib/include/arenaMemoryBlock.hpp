#pragma once

#include <MemoryLib/arenaChunkHeader.hpp>
#include <cstdlib>
#include <iostream>

namespace Moon::Test {
class ArenaMemoryBlockFixture;
class ArenaFixture;
}
namespace Moon
{

struct ArenaMemoryBlock
{
   public:
    ArenaMemoryBlock(const size_t capacity)
        : mStart(nullptr),
          mCapacity(capacity),
          mOffset(0),
          mChunkHeaders(nullptr)
    {
        mStart = static_cast<std::byte*>(malloc(capacity));
    }

    ArenaChunk* RequestEmptyChunk(const size_t size);

    // Assumes that there is enough space in the memory block for this chunk
    ArenaChunk* CreateNewChunk(const size_t requestedSize, const bool setIsUsed = false);
    size_t GetCapacity() const;
    size_t GetRemainingSize();
    bool CanFit(const size_t requestedSize);
    void Release();

    // Total size = requested size + padding (extra size given to chunk) + header size
    static size_t CalcTotalAllocationSize(const size_t requestedSize);

   private:
    std::byte* mStart;
    uint64_t mCapacity;
    uint64_t mOffset;
    ArenaChunkHeader* mChunkHeaders;

    friend class Moon::Test::ArenaMemoryBlockFixture;
    friend class Moon::Test::ArenaFixture;
};

}  // namespace Moon

