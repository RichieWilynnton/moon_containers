#pragma once

#include <AllocatorLib/heapAllocator.hpp>
#include <CommonLib/math.hpp>

#include <utility>

namespace Moon
{

template <typename T>
T* HeapAllocator<T>::Allocate(size_t size)
{
    return static_cast<T*>(malloc(sizeof(T) * size));
}

template <typename T>
void HeapAllocator<T>::Deallocate(T*& ptr)
{
    // no check for nullptr
    free(ptr);
    ptr = nullptr;
}

template <typename T>
template <typename... Args>
void HeapAllocator<T>::Construct(T* ptr, Args&&... args)
{
    new (ptr) T(std::forward<Args>(args)...);
}

template <typename T>
void HeapAllocator<T>::Destruct(T* ptr) noexcept
{
    // no check for nullptr
    ptr->~T();
}

template <typename T>
size_t HeapAllocator<T>::GetNewCapacity(const size_t numOfElems) noexcept
{
    return Util::Math::NextPowerOfTwo(numOfElems + 1);
}
}  // namespace Moon
