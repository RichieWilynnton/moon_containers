#pragma once

#include <cstddef>
#include <cstdlib>
#include <stdexcept>

namespace Moon
{
/*
 NOTE: UniquePtr with array cannot be used here, because constructors shouldn't
 be called when allocating memory on the heap. By default, new T[] calls the
 default constructor for each element. Malloc has to be used instead. Note:
 Destructor has to be called manually for each element in the array,
 * */
template <typename T>
class Stack
{
   public:
    static const size_t STARTING_CAPACITY;

    Stack()
        : mCapacity(STARTING_CAPACITY),
          mElemCount(0),
          mHead(static_cast<T*>(malloc((sizeof(T) * mCapacity))))
    {
        if (mHead == nullptr)
        {
            throw std::runtime_error(MALLOC_ERR_MSG);
        }
    }
    Stack(Stack& other)
        : mCapacity(other.mCapacity),
          mElemCount(other.mElemCount),
          mHead(static_cast<T*>(malloc((sizeof(T) * mCapacity))))
    {
        if (mHead == nullptr)
        {
            throw std::runtime_error(MALLOC_ERR_MSG);
        }

        for (size_t i = 0; i < mElemCount; ++i)
        {
            mHead[i] = other.mHead[i];
            new (&mHead[i]) T(other.mHead[i]);
        }
    }

    Stack(Stack&& other)
        : mCapacity(other.mCapacity),
          mElemCount(other.mElemCount),
          mHead(other.mHead)
    {
        other.mHead = nullptr;
        other.mCapacity = 0;
        other.mElemCount = 0;
    }

    Stack& operator=(Stack& other)
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
                new (&mHead[i]) T(other.mHead[i]);
            }
            mElemCount = other.mElemCount;
        }
        return *this;
    }

    Stack& operator=(Stack&& other)
    {
        if (this != &other)
        {
            Clear();
            mHead = other.mHead;
            mCapacity = other.mCapacity;
            mElemCount = other.mElemCount;

            other.mHead = nullptr;
            other.mCapacity = 0;
            other.mElemCount = 0;
        }
        return *this;
    }

    ~Stack()
    {
        Clear();
    }

    operator bool() const noexcept;

    template <typename... Args>
    void Emplace(Args&&... args);

    void Push(const T& elem);
    void Push(T&& elem);
    void Pop();
    T& Top() const;
    size_t Size() const noexcept;
    bool Empty() const noexcept;
    void Clear();
   private:
    size_t GetNewCapacity(size_t numOfElems) const noexcept;
    void Reallocate(size_t newCapactity); 

    size_t mCapacity;
    size_t mElemCount;
    T* mHead;

    static constexpr char const* MALLOC_ERR_MSG = "Stack(): malloc error";
};

}  // namespace Moon

#include <stackLib/stack.ipp>

// NOTE: free vs delete
// free doesn't call destructors, while delete does.
// NOTE: Handle exceptions for malloc
