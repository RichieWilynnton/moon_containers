#include <CommonLib/math.hpp>
#include <MemoryLib/arenaChunk.hpp>
#include <MemoryLib/arenaChunkHeader.hpp>
#include <MemoryLib/arenaMemoryBlock.hpp>
#include <climits>
#include <new>

#define PAGE_SIZE 1024  // TODO: Change this according to system
//
// header will always be aligned - let allocation size = 2^n. header size
// alignment = 2m (even) since llocation size > header_size, header_start = 2^n
// - 2m = 2m (2^(n-1) - 1)
//
// if we combine chunk and chunk header, then we need a way to store this info
// within the arena this will require a heap allocation, which defeats the
// purpose of the arena

namespace Moon
{

ArenaChunk* ArenaMemoryBlock::RequestEmptyChunk(const size_t size)
{
    if (mChunkHeaders == nullptr)
    {
        return nullptr;
    }

    auto cur = mChunkHeaders;
    size_t bestSize = INT_MAX;
    ArenaChunkHeader* bestChunkHeader = nullptr;

    while (true)
    {
        if (!cur->mIsUsed && cur->GetCapacity() >= size)
        {
            if (cur->GetCapacity() < bestSize)
            {
                bestSize = cur->GetCapacity();
                bestChunkHeader = cur;
            }
        }
        if (cur->mNext == mChunkHeaders)
        {
            break;
        }
        cur = cur->mNext;
    }

    if (bestChunkHeader)
    {
        bestChunkHeader->mIsUsed = true;
        return static_cast<ArenaChunk*>(bestChunkHeader);
    }
    return nullptr;
}

ArenaChunk* ArenaMemoryBlock::CreateNewChunk(const size_t requestedSize, const bool setIsUsed)
{
    const auto totalSize = CalcTotalAllocationSize(requestedSize);
    const size_t chunkSizeAndPadding = totalSize - sizeof(ArenaChunkHeader);

    const auto chunkPtr = mStart + mOffset;
    const auto headerPtr =
        reinterpret_cast<ArenaChunkHeader*>(chunkPtr + chunkSizeAndPadding);
    new (headerPtr) ArenaChunkHeader(chunkPtr, chunkSizeAndPadding);

    headerPtr->mIsUsed = setIsUsed;

    mOffset += totalSize;

    if (mChunkHeaders)
    {
        mChunkHeaders->mPrev->mNext = headerPtr;
        headerPtr->mPrev = mChunkHeaders->mPrev;
        mChunkHeaders->mPrev = headerPtr;
        headerPtr->mNext = mChunkHeaders;
    }
    else
    {
        headerPtr->mNext = headerPtr;
        headerPtr->mPrev = headerPtr;
        mChunkHeaders = headerPtr;
    }

    return static_cast<ArenaChunk*>(headerPtr);
}

size_t ArenaMemoryBlock::GetRemainingSize()
{
    return mCapacity - mOffset;
}

size_t ArenaMemoryBlock::GetCapacity() const
{
    return mCapacity;
}

bool ArenaMemoryBlock::CanFit(const size_t requestedSize)
{
    return GetRemainingSize() >= CalcTotalAllocationSize(requestedSize);
}

size_t ArenaMemoryBlock::CalcTotalAllocationSize(const size_t requestedSize)
{
    const auto totalSize =
        Util::Math::AlignSize(requestedSize + sizeof(ArenaChunkHeader), 64);
    return totalSize;
}

void ArenaMemoryBlock::Release()
{
    if (mStart)
    {
        free(mStart);
        mStart = nullptr;
        mCapacity = 0;
        mOffset = 0;
        mChunkHeaders = nullptr;
    }
}
}  // namespace Moon
