#pragma once

#include <CommonTestLib/dummyTracker.hpp>

namespace Moon::Common::Test
{
class Dummy
{
   public:
    Dummy()
    {
        tracker->DefaultConstructor();
    }

    Dummy(int v) : value(v)
    {
        tracker->ArgConstructor();
    }

    Dummy(const Dummy& other) : value(other.value)
    {
        tracker->CopyConstructor();
    }

    Dummy(Dummy&& other) : value(other.value)
    {
        tracker->MoveConstructor();
    }

    Dummy& operator=(const Dummy& other)
    {
        tracker->CopyAssignment();
        if (this != &other)
        {
            value = other.value;
        }
        return *this;
    }

    Dummy& operator=(Dummy&& other)
    {
        tracker->MoveAssignment();
        if (this != &other)
        {
            value = other.value;
            other.value = 0;
        }
        return *this;
    }

    ~Dummy()
    {
        tracker->Destructor();
    }

    int value = 0;

    static DummyTracker* tracker;
};
}


