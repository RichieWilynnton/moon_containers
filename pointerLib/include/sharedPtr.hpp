#pragma once

#include <PointerLib/controlBlock.hpp>
#include <cstddef>
#include <utility>

namespace Moon
{
template <typename T>
class SharedPtr
{
    using ControlBlock = ControlBlock<T>;

   public:
    SharedPtr(T* ptr = nullptr)
        : mPtr(ptr), mControlBlock(ptr ? new ControlBlock() : nullptr)
    {
        if (mControlBlock)
        {
            mControlBlock->mReferenceCount.store(1);
        }
    }

    SharedPtr(T* ptr, ControlBlock* controlBlock)
        : mPtr(ptr), mControlBlock(controlBlock)
    {
        if (mControlBlock)
        {
            ++mControlBlock->mReferenceCount;
        }
    }

    SharedPtr(const SharedPtr& other)
        : mPtr(other.mPtr), mControlBlock(other.mControlBlock)
    {
        if (mControlBlock)
        {
            ++mControlBlock->mReferenceCount;
        }
    }

    SharedPtr(SharedPtr&& other)
        : mPtr(other.mPtr), mControlBlock(other.mControlBlock)
    {
        other.mPtr = nullptr;
        other.mControlBlock = nullptr;
    }

    SharedPtr& operator=(const SharedPtr& other)
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

    SharedPtr& operator=(SharedPtr&& other)
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

    ~SharedPtr()
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

    ControlBlock* GetControlBlock() const
    {
        return mControlBlock;
    }

    void Reset(T* ptr = nullptr)
    {
        Release();

        if (ptr)
        {
            mPtr = ptr;
            mControlBlock = new ControlBlock();
            mControlBlock->mReferenceCount.store(1);
        }
        else
        {
            mPtr = nullptr;
            mControlBlock = nullptr;
        }
    }

    // NOTE: issue with just having a combined struct with object and control
    // block is that there are now two different ways to delete the object -
    // either delete both ptr and control block separately or delete the
    // combined struct
    template <typename... Args>
    static SharedPtr<T> MakeShared(Args&&... args)
    {
        struct Combined : public ControlBlock
        {
            alignas(T) unsigned char object[sizeof(T)];
        };

        Combined* block = new Combined();
        new (block->object) T(std::forward<Args>(args)...);

        block->mObjectDeleter = [](T* obj) { obj->~T(); };

        block->mControlBlockDeleter = [](ControlBlock* controlBlock)
        {
            auto combined = static_cast<Combined*>(controlBlock);
            delete combined;
        };

        return SharedPtr<T>(reinterpret_cast<T*>(block->object), block);
    }

   private:
    void Release()
    {
        if (mControlBlock && --mControlBlock->mReferenceCount == 0)
        {
            mControlBlock->mObjectDeleter(mPtr);
            if (mControlBlock->mWeakCount == 0)
            {
                mControlBlock->mControlBlockDeleter(mControlBlock);
            }
        }

        mPtr = nullptr;
        mControlBlock = nullptr;
    }

   private:
    T* mPtr;
    ControlBlock* mControlBlock;
};
}  // namespace Moon
