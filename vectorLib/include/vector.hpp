#pragma once

#include <AllocatorLib/heapAllocator.hpp>
#include <VectorLib/vectorIterator.hpp>
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
    }

    Vector(const size_t size, const T& elem = T())
        : mCapacity(Allocator::GetNewCapacity(size)),
          mElemCount(size),
          mHead(Allocator::Allocate(mCapacity))
    {
        for (size_t i = 0; i < size; ++i)
        {
            Allocator::Construct(mHead + i, elem);
        }
    }

    Vector(const VectorIterator<T>& begin,
           const VectorIterator<T>& end) noexcept

        : mCapacity(Allocator::GetNewCapacity(end - begin)),
          mElemCount(end - begin),
          mHead(Allocator::Allocate(mCapacity))
    {
        for (size_t i = 0; i < mElemCount; ++i)
        {
            Allocator::Construct(mHead + i, *(begin + i));
        }
    }

    // NOTE: The templated constructor is not qualified to be a copy constructor
    // because it is a template, hence why, this constructor is still needed.
    // THIS APPLIES TO ALL THE OTHER DEFAULT CLASS FUNCTIONS
    Vector(const Vector& other) noexcept : Vector(other.Begin(), other.End()) {}

    template <typename OtherAllocator>
    Vector(const Vector<T, OtherAllocator>& other) noexcept
        : Vector(other.Begin(), other.End())
    {
    }

    // TODO: add move constructor for different allocators
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
            AssignFrom(other);
        }
        return *this;
    }

    template <typename OtherAllocator>
    Vector& operator=(const Vector<T, OtherAllocator>& other)
    {
        if (this != &other)
        {
            AssignFrom(other);
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
    void AssignFrom(const Vector& other);

   private:
    static constexpr char const* MALLOC_ERR_MSG = "Vector(): malloc error";
    size_t mCapacity;
    size_t mElemCount;
    T* mHead;
};

}  // namespace Moon

#include <vectorLib/vector.ipp>
