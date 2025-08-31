#pragma once

#include <atomic>
namespace Moon
{

template <typename T>
struct ControlBlock
{
    std::atomic<size_t> mReferenceCount;
    std::atomic<size_t> mWeakCount;
    void (*mObjectDeleter)(T* obj);
    void (*mControlBlockDeleter)(ControlBlock* controlBlock);

    ControlBlock()
        : mReferenceCount(0),
          mWeakCount(0),
          mObjectDeleter([](T* obj) { delete obj; }),
          mControlBlockDeleter([](ControlBlock* controlBlock)
                               { delete controlBlock; })
    {
    }
};

}  // namespace Moon
