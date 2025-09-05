#include <MemoryLib/arena.hpp>
#include <MemoryLib/arenaChunkHeader.hpp>
#include <CommonLib/math.hpp>

#include <cassert>

namespace Moon
{

ArenaChunk* Arena::RequestChunk(const size_t size)
{
    assert(size > 0);

    for (auto& memBlock : mMemoryBlocks)
    {
        auto chunk = memBlock.RequestEmptyChunk(size);
        if (chunk)
            return chunk;

        if (memBlock.CanFit(size))
        {
            return memBlock.CreateNewChunk(size, true);
        }
    }

    const auto newMemoryBlockSize = std::max(mDefaultAllocationSize, ArenaMemoryBlock::CalcTotalAllocationSize(size));
    mMemoryBlocks.emplace_back(newMemoryBlockSize);
    return mMemoryBlocks.back().CreateNewChunk(size, true);
}

void Arena::ReleaseChunk(ArenaChunk* arenaChunk)
{
    if (arenaChunk == nullptr)
    {
        return;
    }

    auto* chunkHeader = static_cast<ArenaChunkHeader*>(arenaChunk);

    if (chunkHeader->mIsUsed)
    {
        chunkHeader->mIsUsed = false;
    }
}

}  // namespace Moon

