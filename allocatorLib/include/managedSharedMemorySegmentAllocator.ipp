#pragma once

#include <AllocatorLib/managedSharedMemorySegmentAllocator.hpp>
#include <CommonLib/math.hpp>

namespace Moon
{

template <typename T>
T* ManagedSharedMemorySegmentAllocator<T>::Allocate(size_t size)
{
    void* ptr = mManagedSegment->find_or_construct<char>(
        mRegionName.c_str())[size * sizeof(T)]('\0');
    return static_cast<T*>(ptr);
}

template <typename T>
void ManagedSharedMemorySegmentAllocator<T>::Deallocate(T*& ptr)
{
    // The pointer should be in the shared memory segment
    if (ptr)
    {
        mManagedSegment->destroy_ptr(ptr);
        ptr = nullptr;
    }
}

template <typename T>
template <typename... Args>
void ManagedSharedMemorySegmentAllocator<T>::Construct(T* ptr, Args&&... args)
{
    if (ptr)
    {
        new (ptr) T(std::forward<Args>(args)...);
    }
}

template <typename T>
void ManagedSharedMemorySegmentAllocator<T>::Destruct(T* ptr) noexcept
{
    if (ptr)
    {
        ptr->~T();
    }
}

template <typename T>
size_t ManagedSharedMemorySegmentAllocator<T>::GetNewCapacity(
    const size_t numOfElems) noexcept
{
    return Util::Math::NextPowerOfTwo(numOfElems + 1);
}
}  // namespace Moon
