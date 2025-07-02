#pragma once

#include <StackLib/stack.hpp>

namespace Moon
{

template <typename T>
void Stack<T>::Push(const T& elem)
{
    if (mElemCount == mCapacity)
    {
        // TODO: increase capacity
    }
    else
    {
        *mHead = elem;
    }
    ++mElemCount;
}
}  // namespace Moon
