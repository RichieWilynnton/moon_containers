#pragma once

#include <atomic>
#include <cstddef>
#include <utility>

namespace Moon
{

template <typename T>
class SharedPtr
{
   private:
    struct ControlBlock
    {
        std::atomic<size_t> mReferenceCount;
        void (*mDeleter)(T* obj, ControlBlock* controlBlock);

        ControlBlock()
            : mReferenceCount(1),
              mDeleter(
                  [](T* obj, ControlBlock* controlBlock)
                  {
                      delete controlBlock;
                      delete obj;
                  })
        {
        }
    };

   public:
    SharedPtr(T* ptr = nullptr)
        : mPtr(ptr), mControlBlock(ptr ? new ControlBlock() : nullptr)
    {
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

        block->mDeleter = [](T* obj, ControlBlock* controlBlock)
        {
            auto combined = static_cast<Combined*>(controlBlock);
            obj->~T();
            delete combined;
        };

        return SharedPtr<T>(reinterpret_cast<T*>(block->object), block);
    }

   private:
    SharedPtr(T* ptr, ControlBlock* controlBlock)
        : mPtr(ptr), mControlBlock(controlBlock)
    {
    }

    void Release()
    {
        if (mControlBlock && --mControlBlock->mReferenceCount == 0)
        {
            mControlBlock->mDeleter(mPtr, mControlBlock);
        }

        mPtr = nullptr;
        mControlBlock = nullptr;
    }

   private:
    T* mPtr;
    ControlBlock* mControlBlock;
};
}  // namespace Moon
