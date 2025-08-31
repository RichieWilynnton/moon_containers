#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <CommonTestLib/dummy.hpp>
#include <CommonTestLib/dummyTracker.hpp>
#include <PointerLib/sharedPtr.hpp>
#include <thread>

namespace Moon::Test
{
using Dummy = Moon::Common::Test::Dummy;

class SharedPtrFixture : public ::testing::Test
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

TEST_F(
    SharedPtrFixture,
    WHEN_shared_ptr_is_created_with_raw_pointer_THEN_old_pointer_is_unaffected)
{
    Dummy* oldPtr = new Dummy(5);
    SharedPtr<Dummy> newPtr(oldPtr);
    EXPECT_NE(oldPtr, nullptr);
}

TEST_F(SharedPtrFixture,
       WHEN_shared_ptr_holds_object_THEN_it_can_be_accessed_via_arrow_operator)
{
    SharedPtr<Dummy> ptr(new Dummy(15));

    EXPECT_NE(ptr.Get(), nullptr);
    EXPECT_EQ(ptr->value, 15);
}

TEST_F(
    SharedPtrFixture,
    WHEN_shared_ptr_holds_object_THEN_object_can_be_accessed_via_dereference_operator)
{
    Dummy* dummyPtr = new Dummy(20);
    SharedPtr<Dummy> ptr(dummyPtr);

    EXPECT_EQ((*ptr).value, 20);
}

TEST_F(SharedPtrFixture,
       WHEN_shared_ptr_holds_object_THEN_get_returns_pointer_to_object)
{
    SharedPtr<Dummy> ptr(new Dummy(10));

    EXPECT_NE(ptr.Get(), nullptr);
    EXPECT_EQ((*ptr.Get()).value, 10);
}

TEST_F(
    SharedPtrFixture,
    WHEN_shared_ptr_is_move_constructed_THEN_old_ptr_is_null_and_new_ptr_owns_object)
{
    SharedPtr<Dummy> ptr(new Dummy(42));

    EXPECT_CALL(*dummyTracker, MoveConstructor()).Times(0);
    SharedPtr<Dummy> movedPtr(std::move(ptr));

    EXPECT_EQ(ptr.Get(), nullptr);
    EXPECT_NE(movedPtr.Get(), nullptr);
    EXPECT_EQ(movedPtr->value, 42);
    BlockExpectations();
}

TEST_F(SharedPtrFixture,
       WHEN_shared_ptr_is_reset_THEN_owned_object_is_destroyed)
{
    SharedPtr<Dummy> ptr(new Dummy(100));

    EXPECT_CALL(*dummyTracker, Destructor()).Times(1);
    ptr.Reset();

    EXPECT_EQ(ptr.Get(), nullptr);
    BlockExpectations();
}

TEST_F(SharedPtrFixture,
       WHEN_shared_ptr_is_reset_with_new_object_THEN_it_owns_new_object)
{
    SharedPtr<Dummy> ptr;

    EXPECT_CALL(*dummyTracker, ArgConstructor()).Times(1);
    ptr.Reset(new Dummy(200));

    EXPECT_NE(ptr.Get(), nullptr);
    EXPECT_EQ(ptr->value, 200);
    BlockExpectations();
}

TEST_F(SharedPtrFixture,
       WHEN_shared_ptr_is_move_assigned_THEN_ownership_is_transferred)
{
    SharedPtr<Dummy> ptr1(new Dummy(10));
    SharedPtr<Dummy> ptr2;

    EXPECT_CALL(*dummyTracker, MoveConstructor()).Times(0);
    ptr2 = std::move(ptr1);

    EXPECT_EQ(ptr1.Get(), nullptr);
    EXPECT_NE(ptr2.Get(), nullptr);
    EXPECT_EQ(ptr2->value, 10);
    BlockExpectations();
}

TEST_F(SharedPtrFixture,
       WHEN_shared_ptr_goes_out_of_scope_THEN_owned_object_is_destroyed)
{
    {
        EXPECT_CALL(*dummyTracker, ArgConstructor()).Times(2);
        SharedPtr<Dummy> ptr1(new Dummy(10));
        SharedPtr<Dummy> ptr2(new Dummy(10));

        EXPECT_CALL(*dummyTracker, Destructor()).Times(2);
    }
    BlockExpectations();
}

TEST_F(
    SharedPtrFixture,
    WHEN_shared_ptr_goes_out_of_scope_AND_reference_count_is_zero_THEN_owned_object_is_destroyed)
{
    {
        EXPECT_CALL(*dummyTracker, ArgConstructor()).Times(1);
        SharedPtr<Dummy> ptr1(new Dummy(10));
        {
            SharedPtr<Dummy> ptr2 = ptr1;
            EXPECT_EQ(ptr1.ReferenceCount(), 2);
            EXPECT_EQ(ptr1->value, 10);
            EXPECT_CALL(*dummyTracker, Destructor()).Times(0);
        }
        EXPECT_NE(ptr1.Get(), nullptr);
        EXPECT_EQ(ptr1.ReferenceCount(), 1);
        EXPECT_EQ(ptr1->value, 10);
        EXPECT_CALL(*dummyTracker, Destructor()).Times(1);
    }
    BlockExpectations();
}

TEST_F(
    SharedPtrFixture,
    WHEN_make_shared_is_called_THEN_it_creates_properly_initialized_shared_ptr)
{
    {
        EXPECT_CALL(*dummyTracker, ArgConstructor()).Times(1);
        auto ptr = SharedPtr<Dummy>::MakeShared(30);
        EXPECT_NE(ptr.Get(), nullptr);
        EXPECT_EQ(ptr->value, 30);
        EXPECT_CALL(*dummyTracker, Destructor()).Times(1);
    }
    BlockExpectations();
}

TEST_F(
    SharedPtrFixture,
    WHEN_concurrent_shared_ptrs_are_copy_constructed_THEN_reference_count_is_thread_safe)
{
    for (int iteration = 0; iteration < 5; ++iteration)
    {
        EXPECT_CALL(*dummyTracker, ArgConstructor()).Times(1);
        auto ptr = SharedPtr<Dummy>::MakeShared(50);
        EXPECT_EQ(ptr.ReferenceCount(), 1);
        bool reachedHundred = false;

        std::vector<std::thread> threads;
        std::vector<std::vector<SharedPtr<Dummy>>> localPtrs(10, std::vector<SharedPtr<Dummy>>(10, nullptr));
        for (int i = 0; i < 10; ++i)
        {
            threads.emplace_back(
                [&, i]()
                {
                    for (int j = 0; j < 10; ++j)
                    {
                        localPtrs[i][j] = ptr; 
                        if (localPtrs[i][j].ReferenceCount() ==
                            100 + 1)  // 1 for the original ptr
                        {
                            reachedHundred = true;
                        }
                        else if (localPtrs[i][j].ReferenceCount() > 100 + 1)
                        {
                            ADD_FAILURE()
                                << "Reference count exceeded expected value";
                        }
                    }
                });
        }

        for (auto& thread : threads)
        {
            thread.join();
        }
        EXPECT_TRUE(reachedHundred);
        EXPECT_CALL(*dummyTracker, Destructor()).Times(1);
    }
}

TEST_F(
    SharedPtrFixture,
    WHEN_concurrent_shared_ptrs_are_resetted_THEN_reference_count_is_thread_safe)
{
    for (int iteration = 0; iteration < 5; ++iteration)
    {
        EXPECT_CALL(*dummyTracker, ArgConstructor()).Times(1);
        auto ptr = SharedPtr<Dummy>::MakeShared(50);
        EXPECT_EQ(ptr.ReferenceCount(), 1);
        bool reachedZero = false;

        std::vector<std::thread> threads;
        std::vector<std::vector<SharedPtr<Dummy>>> localPtrs(10, std::vector<SharedPtr<Dummy>>(10, nullptr));
        for (int i=0; i<10; ++i)
        {
            for (int j=0; j<10; ++j)
            {
                localPtrs[i][j] = ptr; 
            }
        }

        for (int i = 0; i < 10; ++i)
        {
            threads.emplace_back(
                [&, i]()
                {
                    for (int j = 0; j < 10; ++j)
                    {
                        localPtrs[i][j].Reset(); 
                        if (localPtrs[i][j].ReferenceCount() ==
                            0)  
                        {
                            reachedZero = true;
                        }
                        else if (localPtrs[i][j].ReferenceCount() < 0)
                        {
                            ADD_FAILURE()
                                << "Reference count went below zero";
                        }
                    }
                });
        }

        for (auto& thread : threads)
        {
            thread.join();
        }
        EXPECT_TRUE(reachedZero);
        EXPECT_CALL(*dummyTracker, Destructor()).Times(1);
    }
}
}  // namespace Moon::Test
