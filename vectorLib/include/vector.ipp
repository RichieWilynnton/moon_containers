#pragma once
#include <VectorLib/vectorIterator.hpp>
#include <cassert>
#include <stdexcept>
#include <vectorLib/vector.hpp>

namespace Moon
{

template <typename T, typename Allocator>
template <typename... Args>
void Vector<T, Allocator>::EmplaceBack(Args&&... args)
{
    if (mElemCount == mCapacity)
    {
        Reallocate(this->Allocator::GetNewCapacity(mElemCount));
    }
    this->Allocator::Construct(mHead + mElemCount, std::forward<Args>(args)...);
    ++mElemCount;
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::Reserve(size_t size)
{
    if (size > mCapacity)
    {
        Reallocate(this->Allocator::GetNewCapacity(size));
    }
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::PushBack(const T& elem)
{
    if (mElemCount == mCapacity)
    {
        Reallocate(this->Allocator::GetNewCapacity(mElemCount));
    }

    this->Allocator::Construct(mHead + mElemCount, elem);
    ++mElemCount;
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::PushBack(T&& elem)
{
    if (mElemCount == mCapacity)
    {
        Reallocate(this->Allocator::GetNewCapacity(mElemCount));
    }

    this->Allocator::Construct(mHead + mElemCount, std::move(elem));
    ++mElemCount;
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::PopBack()
{
    if (mElemCount == 0)
    {
        throw std::runtime_error("PopBack(): empty vector cannot be popped");
    }
    this->Allocator::Destruct(mHead + mElemCount - 1);
    --mElemCount;
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::Clear()
{
    for (int i = 0; i < mElemCount; ++i)
    {
        this->Allocator::Destruct(mHead + i);
    }
    mElemCount = 0;
}

template <typename T, typename Allocator>
size_t Vector<T, Allocator>::Size() const noexcept
{
    return mElemCount;
}

template <typename T, typename Allocator>
bool Vector<T, Allocator>::Empty() const noexcept
{
    return mElemCount == 0;
}

template <typename T, typename Allocator>
T& Vector<T, Allocator>::Back() const
{
    assert(mElemCount > 0 && "Back(): empty vector access, assertion failed");
    return mHead[mElemCount - 1];
}

template <typename T, typename Allocator>
T& Vector<T, Allocator>::operator[](const size_t index) const noexcept
{
    assert(index < mElemCount &&
           "operator[]: out of bounds vector access, assertion failed");
    // no bounds checking
    return mHead[index];
}

template <typename T, typename Allocator>
T& Vector<T, Allocator>::At(const size_t index) const
{
    if (index >= mElemCount)
    {
        throw std::out_of_range("At(): out of bounds vector access");
    }
    return mHead[index];
}

template <typename T, typename Allocator>
size_t Vector<T, Allocator>::Capacity() const noexcept
{
    return mCapacity;
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::Reallocate(size_t newCapacity, size_t startOffset)
{
    assert(startOffset + mElemCount <= newCapacity &&
           "Reallocate(): Impl error");
    T* newHead = this->Allocator::Allocate(newCapacity);

    for (int i = 0; i < mElemCount; ++i)
    {
        this->Allocator::Construct(newHead + i + startOffset, std::move(mHead[i]));
        this->Allocator::Destruct(mHead + i);
    }

    this->Allocator::Deallocate(mHead);

    mHead = newHead;
    mCapacity = newCapacity;
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::AssignFrom(const Vector& other)
{
    // TODO: figure out how to assign allocators
    Clear();
    this->Allocator::Deallocate(mHead);
    if (mCapacity < other.mElemCount)
    {
        const size_t newCapacity = this->Allocator::GetNewCapacity(other.mElemCount);
        mHead = this->Allocator::Allocate(newCapacity);
        mCapacity = newCapacity;
    }

    for (int i = 0; i < other.mElemCount; ++i)
    {
        this->Allocator::Construct(mHead + i, other.mHead[i]);
    }
    mElemCount = other.mElemCount;
}

template <typename T, typename Allocator>
typename Vector<T, Allocator>::Iterator Vector<T, Allocator>::begin() const
{
    return Iterator{mHead};
}

template <typename T, typename Allocator>
typename Vector<T, Allocator>::Iterator Vector<T, Allocator>::end() const
{
    return Iterator{mHead + mElemCount};
}

template <typename T, typename Allocator>
typename Vector<T, Allocator>::Iterator Vector<T, Allocator>::Begin() const
{
    return begin();
}

template <typename T, typename Allocator>
typename Vector<T, Allocator>::Iterator Vector<T, Allocator>::End() const
{
    return end();
}
}  // namespace Moon
