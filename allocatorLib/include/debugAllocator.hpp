#pragma once

#include <cstddef>
#include <unordered_map>

namespace Moon
{

template <typename T>
class DebugAllocator
{
   public:
    static T* Allocate(size_t size);
    static void Deallocate(T*& ptr);

    template <typename... Args>
    static void Construct(T* ptr, Args&&... args);

    static void Destruct(T* ptr) noexcept;
    static size_t GetNewCapacity(const size_t numOfElems) noexcept;
    static size_t GetStartingCapacity() noexcept
    {
        return 1;
    }

    static void ReportLeaks();

    static std::unordered_map<T*, size_t> mAllocations;
};
}  // namespace Moon

#include <AllocatorLib/debugAllocator.ipp>
