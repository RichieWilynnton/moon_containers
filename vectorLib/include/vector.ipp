#pragma once

#include <VectorLib/vectorIterator.hpp>
#include <cassert>
#include <stdexcept>
#include <vectorLib/vector.hpp>

namespace Moon
{

template <typename T>
template <typename... Args>
void Vector<T>::EmplaceBack(Args&&... args)
{
    if (mElemCount == mCapacity)
    {
        Reallocate(GetNewCapacity(mElemCount));
    }
    new (mHead + mElemCount) T(std::forward<Args>(args)...);
    ++mElemCount;
}

template <typename T>
void Vector<T>::Reserve(size_t size)
{
    if (size > mCapacity)
    {
        Reallocate(GetNewCapacity(size));
    }
}

template <typename T>
void Vector<T>::PushBack(const T& elem)
{
    if (mElemCount == mCapacity)
    {
        Reallocate(GetNewCapacity(mElemCount));
    }

    new (mHead + mElemCount) T(elem);
    ++mElemCount;
}

template <typename T>
void Vector<T>::PushBack(T&& elem)
{
    if (mElemCount == mCapacity)
    {
        Reallocate(GetNewCapacity(mElemCount));
    }

    new (mHead + mElemCount) T(std::move(elem));
    ++mElemCount;
}

template <typename T>
void Vector<T>::PopBack()
{
    if (mElemCount == 0)
    {
        throw std::runtime_error("PopBack(): empty vector cannot be popped");
    }
    mHead[mElemCount - 1].~T();
    --mElemCount;
}

template <typename T>
void Vector<T>::Clear()
{
    for (int i = 0; i < mElemCount; ++i)
    {
        mHead[i].~T();
    }
    mElemCount = 0;
}

template <typename T>
size_t Vector<T>::Size() const noexcept
{
    return mElemCount;
}

template <typename T>
bool Vector<T>::Empty() const noexcept
{
    return mElemCount == 0;
}

template <typename T>
T& Vector<T>::Back() const
{
    if (mElemCount == 0)
    {
        throw std::runtime_error("Back(): empty vector cannot be accessed");
    }
    return mHead[mElemCount - 1];
}

template <typename T>
T& Vector<T>::operator[](const size_t index)
{
    // no bounds checking for performance
    assert(index < mElemCount && "operator[]: out of bounds vector access");
    return &mHead[index];
}

template <typename T>
Vector<T>::operator bool() const noexcept
{
    return mElemCount != 0;
}

template <typename T>
void Vector<T>::Reallocate(size_t newCapacity, size_t startOffset)
{
    assert(startOffset + mElemCount <= newCapacity &&
           "Reallocate(): Impl error");
    T* newHead = static_cast<T*>(malloc(sizeof(T) * newCapacity));
    if (newHead == nullptr)
    {
        throw std::runtime_error(MALLOC_ERR_MSG);
    }

    for (int i = 0; i < mElemCount; ++i)
    {
        new (newHead + i + startOffset) T(std::move(mHead[i]));
    }
    mHead = newHead;
    mCapacity = newCapacity;
}

template <typename T>
size_t Vector<T>::GetNewCapacity(size_t numOfElems) const noexcept
{
    return numOfElems * 2;
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::begin() const
{
    return Iterator{mHead};
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::end() const
{
    return Iterator{mHead + mElemCount};
}

}  // namespace Moon
