#pragma once
#include <VectorLib/vectorIterator.hpp>

namespace Moon
{
template <typename T, typename Allocator>
VectorIterator<T, Allocator>& VectorIterator<T, Allocator>::operator++() noexcept
{
    return ++mPtr, *this;
}

template <typename T, typename Allocator>
VectorIterator<T, Allocator> VectorIterator<T, Allocator>::operator++(int) noexcept
{
    VectorIterator<T, Allocator> temp{mPtr};
    ++mPtr;
    return temp;
}

template <typename T, typename Allocator>
VectorIterator<T, Allocator>& VectorIterator<T, Allocator>::operator--() noexcept
{
    return --mPtr, *this;
}

template <typename T, typename Allocator>
VectorIterator<T, Allocator> VectorIterator<T, Allocator>::operator--(int) noexcept
{
    VectorIterator<T, Allocator> temp{mPtr};
    --mPtr;
    return temp;
}

template <typename T, typename Allocator>
VectorIterator<T, Allocator> VectorIterator<T, Allocator>::operator+(int offset) const noexcept
{
    return VectorIterator<T, Allocator>{mPtr + offset};
}

template <typename T, typename Allocator>
VectorIterator<T, Allocator> VectorIterator<T, Allocator>::operator-(int offset) const noexcept
{
    return VectorIterator<T, Allocator>{mPtr - offset};
}

template <typename T, typename Allocator>
bool VectorIterator<T, Allocator>::operator==(const VectorIterator& other) const noexcept
{
    return mPtr == other.mPtr;
}


template <typename T, typename Allocator>
bool VectorIterator<T, Allocator>::operator!=(const VectorIterator& other) const noexcept
{
    return mPtr != other.mPtr;
}

template <typename T, typename Allocator>
bool VectorIterator<T, Allocator>::operator>(const VectorIterator& other) const noexcept
{
    return mPtr > other.mPtr; 
}

template <typename T, typename Allocator>
bool VectorIterator<T, Allocator>::operator<(const VectorIterator& other) const noexcept
{
    return mPtr < other.mPtr; 
}

template <typename T, typename Allocator>
bool VectorIterator<T, Allocator>::operator>=(const VectorIterator& other) const noexcept
{
    return mPtr >= other.mPtr; 
}

template <typename T, typename Allocator>
bool VectorIterator<T, Allocator>::operator<=(const VectorIterator& other) const noexcept
{
    return mPtr <= other.mPtr; 
}

template <typename T, typename Allocator>
T& VectorIterator<T, Allocator>::operator*()
{
    return *mPtr;
}

template <typename T, typename Allocator>
T* VectorIterator<T, Allocator>::operator->() noexcept
{
    return mPtr;
}

}  // namespace Moon
