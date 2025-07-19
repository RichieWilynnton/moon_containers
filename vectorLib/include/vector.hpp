#pragma once

#include <cstddef>
#include <cstdlib>
#include <stdexcept>

namespace Moon
{

template <typename T>
class Vector
{
   public:
    Vector() noexcept
        : mCapacity(DEFAULT_CAPACITY),
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
            mHead[i] = other.mHead[i];
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
                mHead[i] = other.mHead[i];
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
    void EmplaceBack();

    template <typename... Args>
    void EmplaceFront();

    void Reserve();
    void PushBack(const T& elem);
    void PushBack(T&& elem);
    void PopBack();
    void PushFront();
    void PopFront();
    void Clear();
    size_t Size() const noexcept;
    bool Empty() const noexcept;

    T& operator[](const size_t index);

   private:
    size_t GetNewCapacity(size_t numOfElems);

    void Reallocate(size_t newCapacity);

    constexpr static size_t DEFAULT_CAPACITY = 10;
    static constexpr char const* MALLOC_ERR_MSG = "Vector(): malloc error";

    size_t mCapacity;
    size_t mElemCount;
    T* mHead;
};

}  // namespace Moon

#include <vectorLib/vector.ipp>
