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
    static constexpr size_t STARTING_CAPACITY = 8;
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

    static size_t GetNewCapacity(const size_t numOfElems) noexcept
    {
        return Util::Math::NextPowerOfTwo(numOfElems);
    }
};
}  // namespace Moon
