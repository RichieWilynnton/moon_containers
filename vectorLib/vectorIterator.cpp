#include <VectorLib/vectorIterator.hpp>

namespace Moon
{

template <typename T>
VectorIterator<T>& VectorIterator<T>::operator++() noexcept
{
    return ++mPtr, *this;
}

template <typename T>
VectorIterator<T> VectorIterator<T>::operator++(int) noexcept
{
    VectorIterator<T> temp{mPtr};
    ++mPtr;
    return temp;
}

template <typename T>
VectorIterator<T>& VectorIterator<T>::operator--() noexcept
{
    return --mPtr, *this;
}

template <typename T>
VectorIterator<T> VectorIterator<T>::operator--(int) noexcept
{
    VectorIterator<T> temp{mPtr};
    --mPtr;
    return temp;
}

template <typename T>
VectorIterator<T> VectorIterator<T>::operator+(int offset) const noexcept
{
    return VectorIterator<T>{mPtr + offset};
}

template <typename T>
VectorIterator<T> VectorIterator<T>::operator-(int offset) const noexcept
{
    return VectorIterator<T>{mPtr - offset};
}

template <typename T>
bool VectorIterator<T>::operator==(const VectorIterator& other) const noexcept
{
    return mPtr == other.mPtr;
}

template <typename T>
bool VectorIterator<T>::operator>(const VectorIterator& other) const noexcept
{
    return mPtr > other.mPtr; 
}

template <typename T>
bool VectorIterator<T>::operator<(const VectorIterator& other) const noexcept
{
    return mPtr < other.mPtr; 
}

template <typename T>
bool VectorIterator<T>::operator>=(const VectorIterator& other) const noexcept
{
    return mPtr >= other.mPtr; 
}

template <typename T>
bool VectorIterator<T>::operator<=(const VectorIterator& other) const noexcept
{
    return mPtr <= other.mPtr; 
}

template <typename T>
T& VectorIterator<T>::operator*()
{
    return *mPtr;
}

}  // namespace Moon
