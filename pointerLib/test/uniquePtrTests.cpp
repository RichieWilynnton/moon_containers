#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <CommonTestLib/dummy.hpp>
#include <CommonTestLib/dummyTracker.hpp>
#include <PointerLib/uniquePtr.hpp>

namespace Moon::Test
{
using Dummy = Moon::Common::Test::Dummy;

class UniquePtrFixture : public ::testing::Test
{
protected:
    void SetUp() override
    {
        ::testing::GTEST_FLAG(throw_on_failure) = true;
        ::testing::InitGoogleMock();
        dummyTracker = new DummyTracker();
        Dummy::tracker = dummyTracker;
    }

    void TearDown() override
    {
        delete dummyTracker;
        Dummy::tracker = nullptr;
    }

    void BlockExpectations()
    {
        ::testing::Mock::VerifyAndClearExpectations(dummyTracker);
    }

    DummyTracker* dummyTracker;
};

TEST_F(UniquePtrFixture, WHEN_unique_ptr_is_created_with_raw_pointer_THEN_old_pointer_is_unaffected)
{
    Dummy* oldPtr = new Dummy(5);
    UniquePtr<Dummy> newPtr(oldPtr);
    EXPECT_NE(oldPtr, nullptr);    
}

TEST_F(UniquePtrFixture, WHEN_unique_ptr_holds_object_THEN_it_can_be_accessed_via_arrow_operator)
{
    UniquePtr<Dummy> ptr(new Dummy(15));
    
    EXPECT_NE(ptr.Get(), nullptr);
    EXPECT_EQ(ptr->value, 15);
}

TEST_F(UniquePtrFixture, WHEN_unique_ptr_holds_object_THEN_object_can_be_accessed_via_dereference_operator)
{
    Dummy* dummyPtr = new Dummy(20);
    UniquePtr<Dummy> ptr(dummyPtr);
    
    EXPECT_EQ((*ptr).value, 20);
}

TEST_F(UniquePtrFixture, WHEN_unique_ptr_holds_object_THEN_get_returns_pointer_to_object)
{
    UniquePtr<Dummy> ptr(new Dummy(10));
    
    EXPECT_NE(ptr.Get(), nullptr);
    EXPECT_EQ((*ptr.Get()).value, 10);
}

TEST_F(UniquePtrFixture, WHEN_unique_ptr_is_move_constructed_THEN_ownership_is_transferred)
{
    UniquePtr<Dummy> ptr(new Dummy(42));
    
    EXPECT_CALL(*dummyTracker, MoveConstructor()).Times(0);
    UniquePtr<Dummy> movedPtr(std::move(ptr));
    
    EXPECT_EQ(ptr.Get(), nullptr);
    EXPECT_NE(movedPtr.Get(), nullptr);
    EXPECT_EQ(movedPtr->value, 42);
    BlockExpectations();
}

TEST_F(UniquePtrFixture, WHEN_unique_ptr_is_reset_THEN_owned_object_is_destroyed)
{
    UniquePtr<Dummy> ptr(new Dummy(100));
    
    EXPECT_CALL(*dummyTracker, Destructor()).Times(1);
    ptr.Reset();
    
    EXPECT_EQ(ptr.Get(), nullptr);
    BlockExpectations();
}

TEST_F(UniquePtrFixture, WHEN_unique_ptr_is_reset_with_new_object_THEN_it_owns_new_object)
{
    UniquePtr<Dummy> ptr;
    
    EXPECT_CALL(*dummyTracker, ArgConstructor()).Times(1);
    ptr.Reset(new Dummy(200));
    
    EXPECT_NE(ptr.Get(), nullptr);
    EXPECT_EQ(ptr->value, 200);
    BlockExpectations();
}

TEST_F(UniquePtrFixture, WHEN_unique_ptr_release_is_called_THEN_ownership_is_relinquished)
{
    UniquePtr<Dummy> ptr(new Dummy(200));
    
    Dummy* raw = ptr.Release();
    
    EXPECT_EQ(ptr.Get(), nullptr);
    EXPECT_NE(raw, nullptr);
    
    EXPECT_CALL(*dummyTracker, Destructor()).Times(1);
    delete raw;
    BlockExpectations();
}

TEST_F(UniquePtrFixture, WHEN_unique_ptr_is_move_assigned_THEN_ownership_is_transferred)
{
    UniquePtr<Dummy> ptr1(new Dummy(10));
    UniquePtr<Dummy> ptr2;
    
    EXPECT_CALL(*dummyTracker, MoveConstructor()).Times(0);
    ptr2 = std::move(ptr1);
    
    EXPECT_EQ(ptr1.Get(), nullptr);
    EXPECT_NE(ptr2.Get(), nullptr);
    EXPECT_EQ(ptr2->value, 10);
    BlockExpectations();
}

TEST_F(UniquePtrFixture, WHEN_unique_ptr_goes_out_of_scope_THEN_owned_object_is_destroyed)
{
    {
        EXPECT_CALL(*dummyTracker, ArgConstructor()).Times(2);
        UniquePtr<Dummy> ptr1(new Dummy(10));
        UniquePtr<Dummy> ptr2(new Dummy(10));
        
        EXPECT_CALL(*dummyTracker, Destructor()).Times(2);
    }
    BlockExpectations();
}

TEST_F(UniquePtrFixture, WHEN_unique_ptr_array_is_created_THEN_it_manages_array_properly)
{
    UniquePtr<Dummy[]> arr(new Dummy[3]{Dummy(1), Dummy(2), Dummy(3)});
    
    EXPECT_NE(arr.Get(), nullptr);
    EXPECT_EQ(arr[0].value, 1);
    EXPECT_EQ(arr[1].value, 2);
    EXPECT_EQ(arr[2].value, 3);

    EXPECT_CALL(*dummyTracker, Destructor()).Times(3);
    arr.Reset();
    EXPECT_EQ(arr.Get(), nullptr);
    BlockExpectations();
}

TEST_F(UniquePtrFixture, WHEN_unique_ptr_is_used_in_boolean_context_THEN_it_returns_ownership_state)
{
    UniquePtr<Dummy> dummy1;
    UniquePtr<Dummy[]> dummy2;
    
    EXPECT_FALSE(dummy1);
    EXPECT_FALSE(dummy2);
    
    dummy1.Reset(new Dummy(5));
    
    dummy2.Reset(new Dummy[3]{Dummy(1), Dummy(2), Dummy(3)});
    
    EXPECT_TRUE(dummy1);
    EXPECT_TRUE(dummy2);
}

TEST_F(UniquePtrFixture, WHEN_make_unique_is_called_THEN_it_creates_properly_initialized_unique_ptr)
{
    EXPECT_CALL(*dummyTracker, ArgConstructor()).Times(1);
    auto ptr = UniquePtr<Dummy>::MakeUnique(30);
    
    EXPECT_NE(ptr.Get(), nullptr);
    EXPECT_EQ(ptr->value, 30);
    BlockExpectations();
}

TEST_F(UniquePtrFixture, WHEN_make_unique_array_is_called_THEN_it_creates_properly_initialized_array)
{
    auto arr_ptr = UniquePtr<Dummy[]>::MakeUnique(3);
    
    arr_ptr[0] = Dummy(1);
    arr_ptr[1] = Dummy(2);
    arr_ptr[2] = Dummy(3);
    
    EXPECT_NE(arr_ptr.Get(), nullptr);
    EXPECT_EQ(arr_ptr[0].value, 1);
    EXPECT_EQ(arr_ptr[1].value, 2);
    EXPECT_EQ(arr_ptr[2].value, 3);
    BlockExpectations();
}

}  // namespace Moon::Test
