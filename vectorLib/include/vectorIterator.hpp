#pragma once

namespace Moon
{

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
    bool operator>(const VectorIterator& other) const noexcept;
    bool operator<(const VectorIterator& other) const noexcept;
    bool operator>=(const VectorIterator& other) const noexcept;
    bool operator<=(const VectorIterator& other) const noexcept;

    T& operator*();

   private:
    VectorIterator() noexcept;
    T* mPtr;
};
}  // namespace Moon
