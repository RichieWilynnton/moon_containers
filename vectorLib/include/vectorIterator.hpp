#pragma once

namespace Moon
{

template <typename T>
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

    friend class Vector<T>;  
};
}  // namespace Moon

#include <vectorLib/vectorIterator.ipp>
