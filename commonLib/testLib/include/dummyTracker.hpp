#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>


struct DummyTracker
{
    MOCK_METHOD(void, DefaultConstructor, ());
    MOCK_METHOD(void, ArgConstructor, ());
    MOCK_METHOD(void, CopyConstructor, ());
    MOCK_METHOD(void, MoveConstructor, ());
    MOCK_METHOD(void, CopyAssignment, ());
    MOCK_METHOD(void, MoveAssignment, ());
    MOCK_METHOD(void, Destructor, ());
};
