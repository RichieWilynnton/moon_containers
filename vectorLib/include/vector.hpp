#pragma once

#include <VectorLib/vectorIterator.hpp>
#include <cstddef>
#include <cstdlib>
#include <stdexcept>

namespace Moon
{

template <typename T>
class Vector
{
    using Iterator = VectorIterator<T>;

   public:
    constexpr static size_t STARTING_CAPACITY = 10;

   public:
    Vector()
        : mCapacity(STARTING_CAPACITY),
          mElemCount(0),
          mHead(static_cast<T*>(malloc(sizeof(T) * mCapacity)))
    {
        if (mHead == nullptr)
        {
            throw std::runtime_error(MALLOC_ERR_MSG);
        }
    }

    Vector(const Vector& other) noexcept
        : mCapacity(other.mCapacity),
          mElemCount(other.mElemCount),
          mHead(static_cast<T*>(malloc(sizeof(T) * mCapacity)))
    {
        for (int i = 0; i < other.mElemCount; ++i)
        {
            new (mHead + i) T(other.mHead[i]);
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
            if (mCapacity < other.mElemCount)
            {
                const size_t newCapacity = GetNewCapacity(other.mElemCount);
                mHead = static_cast<T*>(malloc(sizeof(T) * newCapacity));
                if (mHead == nullptr)
                {
                    throw std::runtime_error(MALLOC_ERR_MSG);
                }
                mCapacity = newCapacity;
            }

            for (int i = 0; i < other.mElemCount; ++i)
            {
                new (mHead + i) T(other.mHead[i]);
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
    }

    template <typename... Args>
    void EmplaceBack(Args&&... args);

    void Reserve(size_t size);
    void PushBack(const T& elem);
    void PushBack(T&& elem);
    void PopBack();
    void Clear();
    size_t Size() const noexcept;
    bool Empty() const noexcept;
    T& Back() const;

    T& operator[](const size_t index);
    operator bool() const noexcept;

    Iterator begin() const;
    Iterator end() const;

   private:
    size_t GetNewCapacity(size_t numOfElems) const noexcept;
    void Reallocate(size_t newCapacity, size_t startOffset = 0);

   private:
    static constexpr char const* MALLOC_ERR_MSG = "Vector(): malloc error";
    size_t mCapacity;
    size_t mElemCount;
    T* mHead;
};

}  // namespace Moon

#include <vectorLib/vector.ipp>
