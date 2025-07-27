#pragma once

#include <AllocatorLib/debugAllocator.hpp>
#include <AllocatorLib/heapAllocator.hpp>
#include <string>
#include <utility>

namespace Moon
{
template <typename T>
std::unordered_map<T*, size_t> DebugAllocator<T>::mAllocations;

template <typename T>
T* DebugAllocator<T>::Allocate(size_t size)
{
    if (size == 0)
    {
        throw std::runtime_error(
            "DebugAllocator::Allocate(): size must be greater than 0");
    }

    T* ptr = HeapAllocator<T>::Allocate(size);
    if (ptr == nullptr)
    {
        throw std::runtime_error(
            "DebugAllocator::Allocate(): memory allocation failed");
    }

    mAllocations[ptr] = size;

    return ptr;
}

template <typename T>
void DebugAllocator<T>::Deallocate(T*& ptr)
{
    if (!ptr)
        return;

    auto it = mAllocations.find(ptr);
    if (it == mAllocations.end())
    {
        throw std::runtime_error(
            "DebugAllocator::Deallocate(): tried to deallocate unallocated memory");
    }

    mAllocations.erase(it);

    HeapAllocator<T>::Deallocate(ptr);
}

template <typename T>
template <typename... Args>
void DebugAllocator<T>::Construct(T* ptr, Args&&... args)
{
    HeapAllocator<T>::Construct(ptr, std::forward<Args>(args)...);
}

template <typename T>
void DebugAllocator<T>::Destruct(T* ptr) noexcept
{
    HeapAllocator<T>::Destruct(ptr);
}

template <typename T>
size_t DebugAllocator<T>::GetNewCapacity(const size_t numOfElems) noexcept
{
    return HeapAllocator<T>::GetNewCapacity(numOfElems);
}

template <typename T>
void DebugAllocator<T>::ReportLeaks()
{
    if (mAllocations.empty())
    {
        return;
    }

    std::string leakReport = "Memory leaks detected:\n";
    for (const auto& [ptr, size] : mAllocations)
    {
        leakReport +=
            "Pointer: " + std::to_string(reinterpret_cast<size_t>(ptr)) +
            ", Size: " + std::to_string(size) + "\n";
    }

    mAllocations.clear();
    throw std::runtime_error(leakReport);
}
}  // namespace Moon
