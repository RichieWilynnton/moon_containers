#pragma once

#include <CommonLib/math.hpp>
#include <cstddef>
#include <cstdlib>
#include <utility>  

namespace Moon
{
// No bounds checking, this is to maximize performance
template <typename T>
class HeapAllocator
{
   public:
    static constexpr size_t STARTING_CAPACITY = 1;
    static T* Allocate(size_t size)
    {
        return static_cast<T*>(malloc(sizeof(T) * size));
    }

    static void Deallocate(T*& ptr, size_t size)
    {
        // no check for nullptr
        free(ptr);
        ptr = nullptr;
    }

    template <typename... Args>
    static void Construct(T* ptr, Args&&... args)
    {
        new (ptr) T(std::forward<Args>(args)...);
    }

    static void Destruct(T* ptr) noexcept
    {
        // no check for nullptr
        ptr->~T();  
    }

    static size_t GetNewCapacity(const size_t numOfElems) noexcept
    {
        return Util::Math::NextPowerOfTwo(numOfElems + 1);
    }
};
}  // namespace Moon
//
// NOTE: Benefits of having allocators
// 1. Flexible memory management: Allocators allow you to customize how memory is allocated and deallocated, which can lead to more controlled memory usage.
// 2. Easier debugging: memory leaks and allocation issues can be easier to track down

