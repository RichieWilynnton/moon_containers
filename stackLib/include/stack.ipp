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
        Reallocate(GetNewCapacity(mElemCount));
    }
    new (&mHead[mElemCount]) T(std::forward<Args>(args)...);
    ++mElemCount;
}

template <typename T>
void Stack<T>::Push(const T& elem)
{
    if (mElemCount == mCapacity)
    {
        size_t newCapacity = GetNewCapacity(mElemCount);
        T* newHead = static_cast<T*>(malloc(sizeof(T) * newCapacity));
        if (newHead == nullptr)
        {
            Reallocate(GetNewCapacity(mElemCount));
        }
    }
    new (&mHead[mElemCount]) T(elem);
    ++mElemCount;
}

template <typename T>
void Stack<T>::Push(T&& elem)
{
    if (mElemCount == mCapacity)
    {
        Reallocate(GetNewCapacity(mElemCount));
    }
    new (&mHead[mElemCount]) T(std::move(elem));
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
void Stack<T>::Clear()
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
size_t Stack<T>::GetNewCapacity(size_t numOfElems) const noexcept
{
    return mCapacity == 0 ? 1 : mCapacity * 2;
}

template <typename T>
void Stack<T>::Reallocate(size_t newCapacity)
{
    T* newHead = static_cast<T*>(malloc(sizeof(T) * newCapacity));
    if (newHead == nullptr)
    {
        throw std::runtime_error(MALLOC_ERR_MSG);
    }

    for (size_t i = 0; i < mElemCount; ++i)
    {
        new (&newHead[i]) T(std::move(mHead[i]));
    }

    free(mHead);
    mHead = newHead;
    mCapacity = newCapacity;
}

}  // namespace Moon

// mHead[mElemCount] = elem;
// NOTE: I did this in Push(), this is not good because it will call move/copy
// assignment of T, and that object is already garbage. T's assignment
// implementation may refer to things in the old object, so this is dangerous.
// The right way is to use placement new and use the constructors instead of
// assignmnent. NOTE: There are 2 ways to handle reallocation:
// 1. Use move semantics to move existing elements to the new memory location.
// - this will be more efficient, but is not exception safe. If an exception is
// thrown during the move of an element
//   the stack will be in an inconsistent state. This also requires that T
//   has a move constructor and move assignment operator.
// 2. Use copy semantics to copy existing elements to the new memory
// location.
// - this will be less efficient, but is exception safe. If an exception is
// thrown during the copy of an element,
// the old elements are still in the old memory location, and we can just
// revert the vector to the old state.
// I'll choose the first option.
