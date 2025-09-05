#pragma once

#include <CommonLib/math.hpp>
#include <MemoryLib/arenaChunk.hpp>
#include <MemoryLib/arenaChunkHeader.hpp>
#include <MemoryLib/arenaMemoryBlock.hpp>

#include <cstddef>
#include <cstdlib>
#include <vector>

#define PAGE_SIZE 1024  // TODO: Change this according to system

namespace Moon::Test
{
class ArenaFixture;
}  // namespace Moon::Test

namespace Moon
{

class Arena
{
   public:
    Arena(const size_t minAllocationSize)
        : mDefaultAllocationSize(
              Util::Math::AlignSize(minAllocationSize, PAGE_SIZE))
    {
    }
    Arena(const Arena&) = delete;
    Arena(Arena&&) = delete;
    Arena& operator=(const Arena&) = delete;
    Arena& operator=(Arena&&) = delete;
    ~Arena()
    {
        for (auto& block : mMemoryBlocks)
        {
            block.Release();
        }
    }

    ArenaChunk* RequestChunk(const size_t size);
    void ReleaseChunk(ArenaChunk* arenaChunk);
    ArenaMemoryBlock ConstructMemoryBlock(const size_t size);

    size_t GetDefaultAllocationSize() const
    {
        return mDefaultAllocationSize;
    }

   private:
    size_t AlignSize(const size_t size, const size_t alignment);

   private:
    size_t mDefaultAllocationSize;
    std::vector<ArenaMemoryBlock> mMemoryBlocks;

    friend class Moon::Test::ArenaFixture;
};
}  // namespace Moon
