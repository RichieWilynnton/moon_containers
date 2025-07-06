#pragma once

#include <utility>
namespace Moon::Test
{

template <typename T>
class CallTracker : public T
{
   public:
    CallTracker() : T()
    {
        ++mNoArgConstructorCalls;
    }

    CallTracker(CallTracker& other) : T(other)
    {
        ++mCopyConstructorCalls;
    }

    CallTracker(CallTracker&& other) : T(std::move(other))
    {
        ++mMoveConstructorCalls;
    }

    template <typename... Args>
    CallTracker(Args&&... args) : T(std::forward<Args>(args)...)
    {
        ++mArgConstructorCalls;
    }

    CallTracker& operator=(const CallTracker& other)
    {
        T::operator=(other);
        ++mCopyAssignmentCalls;
        return *this;
    }

    CallTracker& operator=(const CallTracker&& other)
    {
        T::operator=(std::move(other));
        ++mMoveAssignmentCalls;
        return *this;
    }

    ~CallTracker() {}
    size_t mNoArgConstructorCalls = 0;
    size_t mMoveConstructorCalls = 0;
    size_t mCopyConstructorCalls = 0;
    size_t mMoveAssignmentCalls = 0;
    size_t mCopyAssignmentCalls = 0;
    size_t mArgConstructorCalls = 0;
};

}  // namespace Moon::Test
