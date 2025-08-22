#pragma once

#include <cstddef>
#include <cstdlib>

namespace Moon
{
// No bounds checking, this is to maximize performance
template <typename T>
class HeapAllocator
{
   public:
    // TODO : implement this
    static constexpr size_t STARTING_CAPACITY = 1;
    static T* Allocate(size_t size);

    static void Deallocate(T*& ptr);

    template <typename... Args>
    static void Construct(T* ptr, Args&&... args);

    static void Destruct(T* ptr) noexcept;

    static size_t GetNewCapacity(const size_t numOfElems) noexcept;
};
}  // namespace Moon
#include <AllocatorLib/heapAllocator.ipp>
//
// NOTE: Benefits of having allocators
// 1. Flexible memory management: Allocators allow you to customize how memory is allocated and deallocated, which can lead to more controlled memory usage.
// 2. Easier debugging: memory leaks and allocation issues can be easier to track down

