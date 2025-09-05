#pragma once

#include <AllocatorLib/arenaAllocator.hpp>

namespace Moon
{

template <typename T>
T* ArenaAllocator<T>::Allocate(size_t size)
{
    mCurrentChunk = mArena->RequestChunk(size * sizeof(T));
    return reinterpret_cast<T*>(mCurrentChunk->GetData());
}

template <typename T>
void ArenaAllocator<T>::Deallocate(T*& ptr)
{
    if (ptr == nullptr)
    {
        return;
    }

    mArena->ReleaseChunk(mCurrentChunk);
}

template <typename T>
template <typename... Args>
void ArenaAllocator<T>::Construct(T* ptr, Args&&... args)
{
    new (ptr) T(std::forward<Args>(args)...);
}

template <typename T>
void ArenaAllocator<T>::Destruct(T* ptr) noexcept
{
    ptr->~T();
}

template <typename T>
size_t ArenaAllocator<T>::GetStartingCapacity() const noexcept
{
    return 1;
}

template <typename T>
size_t ArenaAllocator<T>::GetNewCapacity(const size_t numOfElems) noexcept
{
    return Util::Math::NextPowerOfTwo(numOfElems + 1);
}
} // namespace Moon
