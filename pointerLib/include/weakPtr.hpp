#pragma once

#include <PointerLib/controlBlock.hpp>
#include <PointerLib/sharedPtr.hpp>

namespace Moon
{

template <typename T>
class WeakPtr
{
    using ControlBlock = ControlBlock<T>;

   public:
    WeakPtr() : mPtr(nullptr), mControlBlock(nullptr) {}

    WeakPtr(SharedPtr<T> const& sharedPtr)
        : mPtr(sharedPtr.Get()), mControlBlock(sharedPtr.GetControlBlock())
    {
        if (mControlBlock)
        {
            ++mControlBlock->mWeakCount;
        }
    }

    WeakPtr(const WeakPtr& other)
        : mPtr(other.mPtr), mControlBlock(other.mControlBlock)
    {
        if (mControlBlock)
        {
            ++mControlBlock->mWeakCount;
        }
    }

    WeakPtr(WeakPtr&& other)
        : mPtr(other.mPtr), mControlBlock(other.mControlBlock)
    {
        other.mPtr = nullptr;
        other.mControlBlock = nullptr;
    }

    WeakPtr& operator=(const WeakPtr& other)
    {
        if (this != &other)
        {
            Release();
            mPtr = other.mPtr;
            mControlBlock = other.mControlBlock;
            if (mControlBlock)
            {
                ++mControlBlock->mReferenceCount;
            }
        }
        return *this;
    }

    WeakPtr& operator=(WeakPtr&& other)
    {
        if (this != &other)
        {
            Release();
            mPtr = other.mPtr;
            mControlBlock = other.mControlBlock;
        }

        other.mPtr = nullptr;
        other.mControlBlock = nullptr;
        return *this;
    }

    ~WeakPtr()
    {
        Release();
    }

    T& operator*() const noexcept
    {
        return *mPtr;
    }

    T* operator->() const noexcept
    {
        return mPtr;
    }

    T* Get() const noexcept
    {
        return mPtr;
    }

    void Reset()
    {
        Release();
    }

    operator bool() const noexcept
    {
        return mPtr != nullptr;
    }

    size_t ReferenceCount() const
    {
        return mControlBlock ? mControlBlock->mReferenceCount.load() : 0;
    }

    size_t WeakCount() const
    {
        return mControlBlock ? mControlBlock->mWeakCount.load() : 0;
    }

    bool Expired() const
    {
        return ReferenceCount() == 0;
    }

    SharedPtr<T> Lock()
    {
        if (ReferenceCount() > 0)
        {
            return SharedPtr<T>(mPtr, mControlBlock);
        }
        else
        {
            return SharedPtr<T>();
        }
    }

   private:
    void Release()
    {
        if (mControlBlock && --mControlBlock->mWeakCount == 0 &&
            mControlBlock->mReferenceCount == 0)
        {
            mControlBlock->mControlBlockDeleter(mControlBlock);
        }

        mPtr = nullptr;
        mControlBlock = nullptr;
    }

   private:
    T* mPtr;
    ControlBlock* mControlBlock;
};
}  // namespace Moon
