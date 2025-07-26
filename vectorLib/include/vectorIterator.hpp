#pragma once

#include <AllocatorLib/heapAllocator.hpp>

namespace Moon
{

template <typename T, typename Allocator>
class Vector;

template <typename T, typename Allocator = HeapAllocator<T>>
class VectorIterator
{
   public:
    VectorIterator& operator++() noexcept; 
    VectorIterator operator++(int) noexcept; 
    VectorIterator& operator--() noexcept; 
    VectorIterator operator--(int) noexcept; 
    VectorIterator operator+(int offset) const noexcept;
    VectorIterator operator-(int offset) const noexcept;

    bool operator==(const VectorIterator& other) const noexcept;
    bool operator!=(const VectorIterator& other) const noexcept;
    bool operator>(const VectorIterator& other) const noexcept;
    bool operator<(const VectorIterator& other) const noexcept;
    bool operator>=(const VectorIterator& other) const noexcept;
    bool operator<=(const VectorIterator& other) const noexcept;

    T& operator*();
    T* operator->() noexcept;

   private:
    VectorIterator() noexcept;
    VectorIterator(T* ptr) noexcept: mPtr(ptr) {};
    T* mPtr;

    friend class Vector<T, Allocator>;  
};
}  // namespace Moon

#include <vectorLib/vectorIterator.ipp>
