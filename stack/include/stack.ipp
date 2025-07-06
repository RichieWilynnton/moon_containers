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
    mHead[mElemCount] = elem;
    ++mElemCount;
}

template <typename T>
void Stack<T>::Pop()
{
    if (mElemCount == 0)
        throw std::runtime_error("Pop(): empty stack cannot be poppped");
    --mElemCount;
}

template <typename T>
T& Stack<T>::Top() const
{
    if (mElemCount == 0) throw std::runtime_error("Top(): empty stack cannot be accessed");
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
void Stack<T>::Reallocate()
{
    const size_t newCapacity = GetNewCapacity();
    T* newHead = static_cast<T*>(malloc(newCapacity * sizeof(T)));
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
    mHead = newHead;
}

template <typename T>
size_t Stack<T>::GetNewCapacity() const noexcept
{
    return mCapacity == 0 ? 1 : mCapacity * 2;
}
} // namespace Moon
