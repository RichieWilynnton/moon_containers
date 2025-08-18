#pragma once

#include <AllocatorLib/heapAllocator.hpp>

namespace Moon
{

// Forward declaration of the full Vector template
template <typename U, typename Allocator>
class Vector;

template <typename T>
class VectorIterator
{
   public:
    VectorIterator& operator++() noexcept; 
    VectorIterator operator++(int) noexcept; 
    VectorIterator& operator--() noexcept; 
    VectorIterator operator--(int) noexcept; 
    VectorIterator operator+(int offset) const noexcept;
    VectorIterator operator-(int offset) const noexcept;
    size_t operator-(const VectorIterator& other) const noexcept;

    bool operator==(const VectorIterator& other) const noexcept;
    bool operator!=(const VectorIterator& other) const noexcept;
    bool operator>(const VectorIterator& other) const noexcept;
    bool operator<(const VectorIterator& other) const noexcept;
    bool operator>=(const VectorIterator& other) const noexcept;
    bool operator<=(const VectorIterator& other) const noexcept;

    T& operator*();
    T* operator->() noexcept;

   private:
    VectorIterator(T* ptr) noexcept: mPtr(ptr) {};
    T* mPtr;

    template <typename U, typename Allocator>
    friend class Vector;  
};
}  // namespace Moon

#include <vectorLib/vectorIterator.ipp>
