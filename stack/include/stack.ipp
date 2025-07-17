#pragma once

#include <StackLib/stack.hpp>
#include <cassert>
#include <stdexcept>

namespace Moon
{

template <typename T>
const size_t Stack<T>::STARTING_CAPACITY = 8;

template <typename T>
Stack<T>::operator bool() const noexcept
{
    return mElemCount != 0;
}

template <typename T>
template <typename... Args>
void Stack<T>::Emplace(Args&&... args)
{
    if (mElemCount == mCapacity)
    {
        size_t newCapacity = GetNewCapacity();
        T* newHead = static_cast<T*>(malloc(sizeof(T) * newCapacity));
        if (newHead == nullptr)
        {
            throw std::runtime_error(MALLOC_ERR_MSG);
        }

        for (size_t i = 0; i < mElemCount; ++i)
        {
            newHead[i] = std::move(mHead[i]);
        }

        free(mHead);
        mHead = newHead;
        mCapacity = newCapacity;
    }
    new (&mHead[mElemCount]) T(std::forward<Args>(args)...);
    ++mElemCount;
}

template <typename T>
void Stack<T>::Push(const T& elem)
{
    if (mElemCount == mCapacity)
    {
        size_t newCapacity = GetNewCapacity();
        T* newHead = static_cast<T*>(malloc(sizeof(T) * newCapacity));
        if (newHead == nullptr)
        {
            throw std::runtime_error(MALLOC_ERR_MSG);
        }

        for (size_t i = 0; i < mElemCount; ++i)
        {
            newHead[i] = std::move(mHead[i]);
        }

        free(mHead);
        mHead = newHead;
        mCapacity = newCapacity;
    }
    mHead[mElemCount] = elem;
    ++mElemCount;
}

template <typename T>
void Stack<T>::Pop()
{
    if (mElemCount == 0)
        throw std::runtime_error("Pop(): empty stack cannot be poppped");
    mHead[mElemCount - 1].~T();
    --mElemCount;
}

template <typename T>
T& Stack<T>::Top() const
{
    if (mElemCount == 0)
        throw std::runtime_error("Top(): empty stack cannot be accessed");
    return mHead[mElemCount - 1];
}

template <typename T>
size_t Stack<T>::Size() const noexcept
{
    return mElemCount;
}

template <typename T>
bool Stack<T>::Empty() const noexcept
{
    return mElemCount == 0;
}

template <typename T>
void Stack<T>::DeallocateAndDestruct()
{
    if (mHead)
    {
        for (size_t i = 0; i < mElemCount; ++i)
        {
            mHead[i].~T();
        }
        free(mHead);
        mHead = nullptr;
        mCapacity = 0;
        mElemCount = 0;
    }
}

template <typename T>
size_t Stack<T>::GetNewCapacity() const noexcept
{
    return mCapacity == 0 ? 1 : mCapacity * 2;
}
}  // namespace Moon
