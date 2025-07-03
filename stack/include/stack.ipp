#pragma once

#include <StackLib/stack.hpp>
#include <stdexcept>

namespace Moon
{

template <typename T>
void Stack<T>::Push(const T& elem)
{
    if (mElemCount == mCapacity)
    {
        Reallocate();
    }
    *mHead = elem;
    ++mElemCount;
}

template <typename T>
void Stack<T>::Pop(const T& elem)
{
    if (mElemCount == 0)
        throw std::runtime_error("Pop(): empty stack cannot be poppped");
    --mElemCount;
}

template <typename T>
T& Stack<T>::Top() const noexcept
{
    auto ptr = mHead + sizeof(T) * (mElemCount - 1);
    return *ptr;
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
void Stack<T>::Reallocate()
{
    const size_t newCapacity = GetNewCapacity();
    T* newHead = malloc(newCapacity * sizeof(T));

    if (newHead == nullptr)
    {
        throw std::runtime_error("Reallocate(): malloc error");
    }

    T* newTail = newHead + newCapacity;

    for (size_t i = 0; i < newCapacity; ++i) 
    {
       newHead[i] = mHead[i]; 
    }

    mCapacity = newCapacity;

    free(mHead);
    free(mTail);
    mHead = newHead;
    mTail = newTail;
}
}  // namespace Moon
