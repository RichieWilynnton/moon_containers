#pragma once

#include <vectorLib/vector.hpp>

namespace Moon
{

template <typename T>
template <typename... Args>
void Vector<T>::EmplaceBack()
{
}

template <typename T>
template <typename... Args>
void Vector<T>::EmplaceFront()
{
}

template <typename T>
void Vector<T>::Reserve()
{
}

template <typename T>
void Vector<T>::PushBack(const T& elem)
{
    if (mElemCount == mCapacity)
    {
        Reallocate();
    }

    mHead[mElemCount] = elem;
    ++mElemCount;
}


template <typename T>
void Vector<T>::PushBack(T&& elem)
{
    if (mElemCount == mCapacity)
    {
        Reallocate();
    }

    mHead[mElemCount] = elem;
    ++mElemCount;
}

template <typename T>
void Vector<T>::PopBack()
{
}

template <typename T>
void Vector<T>::PushFront()
{
}

template <typename T>
void Vector<T>::PopFront()
{
}

template <typename T>
void Vector<T>::Clear()
{
    for (int i = 0; i < mElemCount; ++i)
    {
        ~mHead[i]();
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
T& Vector<T>::operator[](const size_t index)
{
}

template <typename T>
void Vector<T>::Reallocate(size_t newCapacity)
{
    T* newHead = static_cast<T*>(malloc(sizeof(T) * newCapacity));
    if (newHead == nullptr)
    {
        throw std::runtime_error(MALLOC_ERR_MSG);
    }

    for (int i = 0; i < mElemCount; ++i)
    {
        newHead[i] = std::move(mHead[i]);
    }
    mHead = newHead;
    mCapacity = newCapacity;
}
}  // namespace Moon
