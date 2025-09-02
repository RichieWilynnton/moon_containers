#pragma once

#include <AllocatorLib/arenaAllocator.hpp>

namespace Moon
{

template <typename T>
T* ArenaAllocator<T>::Allocate(size_t size)
{
    // Handled by arena    
}

template <typename T>
void ArenaAllocator<T>::Deallocate(T*& ptr)
{
    // Handled by arena    
}

template <typename T>
template <typename... Args>
void ArenaAllocator<T>::Construct(T* ptr, Args&&... args)
{
    mArena.
}

template <typename T>
void ArenaAllocator<T>::Destruct(T* ptr) noexcept
{
    // no check for nullptr
    ptr->~T();
}

template <typename T>
size_t ArenaAllocator<T>::GetNewCapacity(const size_t numOfElems) noexcept
{
    return Util::Math::NextPowerOfTwo(numOfElems + 1);
}
} // namespace Moon
