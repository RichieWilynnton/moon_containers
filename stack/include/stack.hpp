#pragma once

#include <PointerLib/uniquePtr.hpp>
#include <cstddef>

namespace Moon
{

template <typename T>
class Stack
{
   public:
    Stack()
        : mCapacity(STARTING_CAPACITY),
          mElemCount(0),
          mHead(malloc(sizeof(T) * mCapacity), mTail(mHead)) {};
    ~Stack() 
    {
        free(mHead);
        free(mTail);
    }
    Stack(Stack& other)
    {
        mCapacity = other.mCapacity;
        mElemCount = other.mElemCount;
        // TODO: copy all of the elements from head to tail
    }
    Stack(Stack&& other)
    {
        mCapacity = other.mCapacity;
        mElemCount = other.mElemCount;
        mHead = other.mHead;
        mTail = other.mTail;

        mCapacity = 0;
        mElemCount = 0;
        other.mHead = nullptr;
        other.mTail = nullptr;
    }
    Stack& operator=(Stack& other)
    {
        if (this != &other)
        {
            mCapacity = other.mCapacity;
            mElemCount = other.mElemCount;
            // TODO: copy all of the elements from head to tail
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
            mTail = other.mTail;

            mCapacity = 0;
            mElemCount = 0;
            other.mHead = nullptr;
            other.mTail = nullptr;
        }
    }

    operator bool() const noexcept;

    void Push(const T& elem);
    void Pop(const T& elem);
    T& Top() const noexcept;
    size_t Size() const noexcept;
    bool Empty() const noexcept;


   private:
    void Reallocate();
    size_t GetNewCapacity();

    static const size_t STARTING_CAPACITY = 10;
    size_t mCapacity;
    size_t mElemCount;
    T* mHead;
    T* mTail;
};

}  // namespace Moon
