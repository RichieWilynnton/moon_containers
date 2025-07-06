#pragma once

#include <PointerLib/uniquePtr.hpp>

#include <cstddef>
#include <cstdlib>

namespace Moon
{
/*
 NOTE: UniquePtr with array cannot be used here, because constructors shouldn't be called
    when allocating memory on the heap. By default, new T[] calls the default constructor for each element.
    Malloc has to be used instead.
 * */
template <typename T>
class Stack
{
   public:
    Stack()
        : mCapacity(STARTING_CAPACITY),
          mElemCount(0),
          mHead(static_cast<T*>(malloc((sizeof(T) * mCapacity))))
    {
    }
    ~Stack()
    {
        free(mHead);
    }
    Stack(Stack& other)
    {
        mCapacity = other.mCapacity;
        mElemCount = other.mElemCount;

        for (size_t i = 0; i < other.mCapacity; ++i)
        {
            mHead[i] = other.mHead[i];
        }
    }
    Stack(Stack&& other)
    {
        mCapacity = other.mCapacity;
        mElemCount = other.mElemCount;
        mHead = other.mHead;

        other.mCapacity = 0;
        other.mElemCount = 0;
        other.mHead = nullptr;
        if (other.mHead)
            free(other.mHead);
    }
    Stack& operator=(Stack& other)
    {
        if (this != &other)
        {
            mCapacity = other.mCapacity;
            mElemCount = other.mElemCount;

            if (mHead)
            {
                free(mHead);
            }

            mHead = static_cast<T*>(malloc((sizeof(T) * mCapacity)));
            for (size_t i = 0; i < other.mCapacity; ++i)
            {
                mHead[i] = other.mHead[i];
            }
        }
        return *this;
    }
    Stack& operator=(Stack&& other)
    {
        if (this != &other)
        {
            mCapacity = other.mCapacity;
            mElemCount = other.mElemCount;
            mHead = other.mHead;

            other.mCapacity = 0;
            other.mElemCount = 0;
            other.mHead = nullptr;
            if (other.mHead)
                free(other.mHead);
        }
        return *this;
    }

    operator bool() const noexcept;

    void Push(const T& elem);
    void Pop();
    T& Top() const;
    size_t Size() const noexcept;
    bool Empty() const noexcept;

   private:
    void Reallocate();
    size_t GetNewCapacity() const noexcept;

    static const size_t STARTING_CAPACITY = 10;
    size_t mCapacity;
    size_t mElemCount;
    T* mHead;
};

}  // namespace Moon

#include <stackLib/stack.ipp>

// NOTE: free vs delete
// free doesn't call destructors, while delete does.
