
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <CommonTestLib/dummy.hpp>
#include <CommonTestLib/dummyTracker.hpp>
#include <PointerLib/sharedPtr.hpp>
#include <PointerLib/weakPtr.hpp>
#include <thread>

namespace Moon::Test
{
using Dummy = Moon::Common::Test::Dummy;

class WeakPtrFixture : public ::testing::Test
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
    WeakPtrFixture,
    WHEN_weak_ptr_is_constructed_THEN_weak_ptr_count_is_incremented_AND_reference_count_is_unchanged)
{
    SharedPtr<Dummy> sharedPtr(new Dummy(5));
    EXPECT_EQ(sharedPtr.ReferenceCount(), 1);
    EXPECT_EQ(sharedPtr.WeakCount(), 0);

    {
        WeakPtr<Dummy> weakPtr(sharedPtr);
        EXPECT_EQ(sharedPtr.ReferenceCount(), 1);
        EXPECT_EQ(sharedPtr.WeakCount(), 1);
        EXPECT_CALL(*dummyTracker, Destructor()).Times(0);
    }

    BlockExpectations();
    EXPECT_EQ(sharedPtr.ReferenceCount(), 1);
    EXPECT_EQ(sharedPtr.WeakCount(), 0);
}

TEST_F(WeakPtrFixture,
       WHEN_weak_ptr_holds_object_THEN_it_can_be_accessed_via_arrow_operator)
{
    SharedPtr<Dummy> sharedPtr(new Dummy(5));
    EXPECT_EQ(sharedPtr.ReferenceCount(), 1);
    EXPECT_EQ(sharedPtr.WeakCount(), 0);

    {
        WeakPtr<Dummy> weakPtr(sharedPtr);
        EXPECT_EQ(weakPtr->value, 5);
    }
}

TEST_F(
    WeakPtrFixture,
    WHEN_weak_ptr_holds_object_THEN_object_can_be_accessed_via_dereference_operator)
{
    {
        SharedPtr<Dummy> sharedPtr(new Dummy(5));
        EXPECT_EQ(sharedPtr.ReferenceCount(), 1);
        EXPECT_EQ(sharedPtr.WeakCount(), 0);

        {
            WeakPtr<Dummy> weakPtr(sharedPtr);
            EXPECT_EQ((*weakPtr).value, 5);
        }
    }
}

TEST_F(WeakPtrFixture,
       WHEN_weak_ptr_holds_object_THEN_get_returns_pointer_to_object)
{
    {
        SharedPtr<Dummy> sharedPtr(new Dummy(5));
        EXPECT_EQ(sharedPtr.ReferenceCount(), 1);
        EXPECT_EQ(sharedPtr.WeakCount(), 0);

        {
            WeakPtr<Dummy> weakPtr(sharedPtr);
            EXPECT_NE(weakPtr.Get(), nullptr);
            EXPECT_EQ(weakPtr.Get(), sharedPtr.Get());
        }
    }
}

TEST_F(
    WeakPtrFixture,
    WHEN_weak_ptr_is_move_constructed_THEN_old_ptr_is_null_and_new_ptr_owns_object)
{
    SharedPtr<Dummy> sharedPtr(new Dummy(42));
    EXPECT_EQ(sharedPtr.ReferenceCount(), 1);
    EXPECT_EQ(sharedPtr.WeakCount(), 0);

    WeakPtr<Dummy> weakPtr(sharedPtr);
    EXPECT_EQ(sharedPtr.ReferenceCount(), 1);
    EXPECT_EQ(sharedPtr.WeakCount(), 1);

    EXPECT_CALL(*dummyTracker, MoveConstructor()).Times(0);
    WeakPtr<Dummy> movedPtr(std::move(weakPtr));

    EXPECT_EQ(weakPtr.Get(), nullptr);
    EXPECT_NE(movedPtr.Get(), nullptr);
    EXPECT_EQ(movedPtr->value, 42);
    BlockExpectations();
}

TEST_F(WeakPtrFixture,
       WHEN_weak_ptr_is_move_assigned_THEN_ownership_is_transferred)
{
    SharedPtr<Dummy> sharedPtr(new Dummy(42));
    EXPECT_EQ(sharedPtr.ReferenceCount(), 1);
    EXPECT_EQ(sharedPtr.WeakCount(), 0);

    WeakPtr<Dummy> weakPtr(sharedPtr);
    EXPECT_EQ(sharedPtr.ReferenceCount(), 1);
    EXPECT_EQ(sharedPtr.WeakCount(), 1);

    WeakPtr<Dummy> assignedPtr;
    EXPECT_CALL(*dummyTracker, MoveConstructor()).Times(0);
    assignedPtr = std::move(weakPtr);

    EXPECT_EQ(weakPtr.Get(), nullptr);
    EXPECT_NE(assignedPtr.Get(), nullptr);
    EXPECT_EQ(assignedPtr->value, 42);
    BlockExpectations();
}

TEST_F(
    WeakPtrFixture,
    WHEN_weak_ptr_is_in_scope_AND_shared_ptr_goes_out_of_scope_THEN_expired_is_true)
{
    {
        SharedPtr<Dummy> sharedPtr(new Dummy(10));
        WeakPtr<Dummy> weakPtr(sharedPtr);
        sharedPtr.Reset();
        EXPECT_TRUE(weakPtr.Expired());
    }
    BlockExpectations();
}

TEST_F(
    WeakPtrFixture,
    WHEN_weak_ptr_is_locked_AND_shared_ptr_is_alive_THEN_lock_returns_valid_shared_ptr)
{
    {
        SharedPtr<Dummy> sharedPtr(new Dummy(10));
        WeakPtr<Dummy> weakPtr(sharedPtr);
        auto lockedPtr = weakPtr.Lock();
        EXPECT_NE(lockedPtr.Get(), nullptr);
        EXPECT_EQ(lockedPtr->value, 10);
        EXPECT_EQ(lockedPtr.ReferenceCount(), 2);
        EXPECT_EQ(lockedPtr.WeakCount(), 1);
    }
    BlockExpectations();
}

TEST_F(
    WeakPtrFixture,
    WHEN_weak_ptr_is_locked_AND_shared_ptr_is_destroyed_THEN_lock_returns_nullptr)
{
    {
        SharedPtr<Dummy> sharedPtr(new Dummy(10));
        WeakPtr<Dummy> weakPtr(sharedPtr);
        sharedPtr.Reset();
        auto lockedPtr = weakPtr.Lock();
        EXPECT_EQ(lockedPtr.Get(), nullptr);
        EXPECT_EQ(lockedPtr.ReferenceCount(), 0);
        EXPECT_EQ(lockedPtr.WeakCount(), 0);
    }
    BlockExpectations();
}

TEST_F(
    WeakPtrFixture,
    WHEN_concurrent_weak_ptrs_are_copy_constructed_THEN_reference_count_is_thread_safe)
{
    for (int iteration = 0; iteration < 5; ++iteration)
    {
        EXPECT_CALL(*dummyTracker, ArgConstructor()).Times(1);
        auto ptr = SharedPtr<Dummy>::MakeShared(50);
        EXPECT_EQ(ptr.ReferenceCount(), 1);
        bool reachedHundred = false;

        std::vector<std::thread> threads;
        std::vector<std::vector<WeakPtr<Dummy>>> localPtrs(
            10, std::vector<WeakPtr<Dummy>>(10));
        for (int i = 0; i < 10; ++i)
        {
            threads.emplace_back(
                [&, i]()
                {
                    for (int j = 0; j < 10; ++j)
                    {
                        localPtrs[i][j] = WeakPtr(ptr);
                        std::cout
                            << "Weak Count: " << localPtrs[i][j].WeakCount()
                            << std::endl;
                        if (localPtrs[i][j].WeakCount() == 100)
                        {
                            reachedHundred = true;
                        }
                        else if (localPtrs[i][j].WeakCount() > 100)
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
    WeakPtrFixture,
    WHEN_concurrent_weak_ptrs_are_resetted_THEN_reference_count_is_thread_safe)
{
    for (int iteration = 0; iteration < 5; ++iteration)
    {
        EXPECT_CALL(*dummyTracker, ArgConstructor()).Times(1);
        auto ptr = SharedPtr<Dummy>::MakeShared(50);
        EXPECT_EQ(ptr.ReferenceCount(), 1);
        bool reachedZero = false;

        std::vector<std::thread> threads;
        std::vector<std::vector<WeakPtr<Dummy>>> localPtrs(
            10, std::vector<WeakPtr<Dummy>>(10));
        for (int i = 0; i < 10; ++i)
        {
            for (int j = 0; j < 10; ++j)
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
                        if (localPtrs[i][j].WeakCount() == 0)
                        {
                            reachedZero = true;
                        }
                        else if (localPtrs[i][j].WeakCount() < 0)
                        {
                            ADD_FAILURE() << "Reference count went below zero";
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
