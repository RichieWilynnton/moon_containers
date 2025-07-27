#pragma once

#include <VectorLib/vectorIterator.hpp>
#include <AllocatorLib/heapAllocator.hpp>

#include <stdexcept>
#include <cstddef>

namespace Moon
{

// template <typename T>
template <typename T, typename Allocator = HeapAllocator<T>>
class Vector
{
    using Iterator = VectorIterator<T>;

   public:
    Vector()
        : mCapacity(Allocator::STARTING_CAPACITY),
          mElemCount(0),
          mHead(Allocator::Allocate(mCapacity))
    {
        if (mHead == nullptr)
        {
            throw std::runtime_error(MALLOC_ERR_MSG);
        }
    }

    Vector(const size_t size, const T& elem = T())
        : mCapacity(Allocator::GetNewCapacity(size)),
          mElemCount(size),
          mHead(Allocator::Allocate(mCapacity))
    {
        if (mHead == nullptr)
        {
            throw std::runtime_error(MALLOC_ERR_MSG);
        }
        for (size_t i = 0; i < size; ++i)
        {
             Allocator::Construct(mHead + i, elem);
        }
    }

    // TODO: Perfect forwarding
    // T&& preserves the value category of the argument
    // if the argument is an lvalue, T&& becomes T&,
    // if the argument is an rvalue, T&& becomes T&&.
    // Why not just const Args&, because r-values would then be treated as l-values, leading to copies
    // template <typename... Args>
    // Vector(Args&&... args)
    //     : mCapacity(Allocator::GetNewCapacity(sizeof...(args))),
    //       mElemCount(sizeof...(args)),
    //       mHead(Allocator::Allocate(mCapacity))
    // {
    //     size_t i = 0;
    //     (Allocator::Construct(mHead + i++, std::forward<Args>(args)), ...);
    // }

    // TODO: figure out how to copy elements from vectors with different allocators 
    Vector(const Vector& other) noexcept
        : mCapacity(other.mCapacity),
          mElemCount(other.mElemCount),
          mHead(Allocator::Allocate(mCapacity))
    {
        for (int i = 0; i < other.mElemCount; ++i)
        {
             Allocator::Construct(mHead + i, other.mHead[i]);
        }

    }

    Vector(Vector&& other) noexcept
        : mCapacity(other.mCapacity),
          mElemCount(other.mElemCount),
          mHead(other.mHead)
    {
        other.mHead = nullptr;
        other.mElemCount = 0;
        other.mCapacity = 0;
    }

    Vector& operator=(const Vector& other)
    {
        if (this != &other)
        {
            Clear();
            Allocator::Deallocate(mHead);
            if (mCapacity < other.mElemCount)
            {
                const size_t newCapacity = Allocator::GetNewCapacity(other.mElemCount);
                mHead = Allocator::Allocate(newCapacity);
                mCapacity = newCapacity;
            }

            for (int i = 0; i < other.mElemCount; ++i)
            {
                 Allocator::Construct(mHead + i, other.mHead[i]);
            }
            mElemCount = other.mElemCount;
        }
        return *this;
    }

    Vector& operator=(Vector&& other)
    {
        if (this != &other)
        {
            Clear();
            mHead = other.mHead;
            mCapacity = other.mCapacity;
            mElemCount = other.mElemCount;

            other.mHead = nullptr;
            other.mElemCount = 0;
            other.mCapacity = 0;
        }
        return *this;
    }

    ~Vector()
    {
        Clear();
        Allocator::Deallocate(mHead);
    }

    template <typename... Args>
    void EmplaceBack(Args&&... args);

    void Reserve(size_t size);
    void PushBack(const T& elem);
    void PushBack(T&& elem);
    void PopBack();
    void Clear();
    size_t Capacity() const noexcept;
    size_t Size() const noexcept;
    bool Empty() const noexcept;
    T& Back() const;
    T& At(const size_t index) const;

    T& operator[](const size_t index) const noexcept;

    Iterator begin() const;
    Iterator end() const;
    Iterator Begin() const;
    Iterator End() const;

   private:
    void Reallocate(size_t newCapacity, size_t startOffset = 0);

   private:
    static constexpr char const* MALLOC_ERR_MSG = "Vector(): malloc error";
    size_t mCapacity;
    size_t mElemCount;
    T* mHead;
};

}  // namespace Moon

#include <vectorLib/vector.ipp>

