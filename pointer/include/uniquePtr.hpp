#pragma once

#include <utility>

namespace Moon
{

template <typename T>
class UniquePtr
{
   public:
    UniquePtr() : mPtr(nullptr) {};
    explicit UniquePtr(T *ptr) : mPtr(ptr)
    {
        ptr = nullptr;
    };

    ~UniquePtr()
    {
        if (mPtr)
        {
            delete mPtr;
        }
    }
    UniquePtr(UniquePtr &other) = delete;
    UniquePtr(UniquePtr &&other) noexcept : mPtr(other.mPtr)
    {
        other.mPtr = nullptr;
    }
    UniquePtr &operator=(UniquePtr &other) = delete;
    UniquePtr &operator=(UniquePtr &&other) noexcept
    {
        if (this != &other)
        {
            delete mPtr;
            mPtr = other.mPtr;
            other.mPtr = nullptr;
        }
        return *this;
    }

    T *Release()
    {
        T *ptr = mPtr;
        mPtr = nullptr;
        return ptr;
    }

    void Reset(T *ptr = nullptr) noexcept
    {
        if (mPtr)
        {
            delete mPtr;
        }
        mPtr = ptr;
    }
    T *Get() const
    {
        return mPtr;
    }
    T &operator*() const
    {
        return *mPtr;
    }
    T *operator->() const
    {
        return mPtr;
    }

    operator bool() const noexcept
    {
        return mPtr != nullptr;
    }

    template <typename... Args>
    static UniquePtr<T> MakeUnique(Args &&...args)
    {
        return UniquePtr<T>(new T(std::forward<Args>(args)...));
    }

   private:
    T *mPtr;
};

template <typename T>
class UniquePtr<T[]>
{
   public:
    UniquePtr() : mPtr(nullptr) {};
    explicit UniquePtr(T *ptr) : mPtr(ptr) {};

    ~UniquePtr()
    {
        if (mPtr)
        {
            delete[] mPtr;
        }
    }
    UniquePtr(UniquePtr &other) = delete;
    UniquePtr(UniquePtr &&other) noexcept : mPtr(other.mPtr)
    {
        other.mPtr = nullptr;
    }
    UniquePtr &operator=(UniquePtr &other) = delete;
    UniquePtr &operator=(UniquePtr &&other) noexcept
    {
        if (this != &other)
        {
            delete[] mPtr;
            mPtr = other.mPtr;
            other.mPtr = nullptr;
        }
        return *this;
    }

    T *Release()
    {
        T *ptr = mPtr;
        mPtr = nullptr;
        return ptr;
    }

    void Reset(T *ptr = nullptr) noexcept
    {
        if (mPtr)
        {
            delete[] mPtr;
        }
        mPtr = ptr;
    }
    T *Get() const
    {
        return mPtr;
    }
    T &operator*() const
    {
        return *mPtr;
    }
    T *operator->() const
    {
        return mPtr;
    }

    operator bool() const noexcept
    {
        return mPtr != nullptr;
    }

    T &operator[](int index)
    {
        return mPtr[index];
    }

    static UniquePtr<T[]> MakeUnique(size_t size)
    {
        return UniquePtr<T[]>(new T[size]());
    }

   private:
    T *mPtr;
};
}  // namespace Moon
