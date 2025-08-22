#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <CommonTestLib/dummy.hpp>
#include <CommonTestLib/dummyTracker.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <memory>

#include "AllocatorLib/managedSharedMemorySegmentAllocator.hpp"

namespace Moon::Test
{
using Dummy = Moon::Common::Test::Dummy;

class ManagedSharedMemorySegmentAllocatorFixture : public ::testing::Test
{
   protected:
    void SetUp() override
    {
        ::testing::GTEST_FLAG(throw_on_failure) = true;
        ::testing::InitGoogleMock();
        dummyTracker = new DummyTracker();
        Dummy::tracker = dummyTracker;

        mSegment = std::make_shared<boost::interprocess::managed_shared_memory>(
            boost::interprocess::open_or_create, "TestSegment", 65536);
    }

    void TearDown() override
    {
        boost::interprocess::shared_memory_object::remove("TestSegment");

        delete dummyTracker;
        Dummy::tracker = nullptr;
    }

    void BlockExpectations()
    {
        ::testing::Mock::VerifyAndClearExpectations(dummyTracker);
    }

    DummyTracker* dummyTracker;
    std::shared_ptr<boost::interprocess::managed_shared_memory> mSegment;
};

TEST_F(ManagedSharedMemorySegmentAllocatorFixture,
       WHEN_allocate_is_called_THEN_elements_are_not_constructed)
{
    ManagedSharedMemorySegmentAllocator<Dummy> allocator(mSegment,
                                                         "TestVector");

    EXPECT_CALL(*dummyTracker, DefaultConstructor()).Times(0);
    EXPECT_CALL(*dummyTracker, ArgConstructor()).Times(0);
    EXPECT_CALL(*dummyTracker, CopyConstructor()).Times(0);
    EXPECT_CALL(*dummyTracker, Destructor()).Times(0);

    EXPECT_NE(allocator.Allocate(3), nullptr);
}

TEST_F(ManagedSharedMemorySegmentAllocatorFixture,
       WHEN_construct_is_called_THEN_elements_are_constructed_in_place)
{
    ManagedSharedMemorySegmentAllocator<Dummy> allocator(mSegment,
                                                         "TestVector");

    Dummy* ptr = allocator.Allocate(2);

    EXPECT_CALL(*dummyTracker, DefaultConstructor()).Times(0);
    EXPECT_CALL(*dummyTracker, ArgConstructor()).Times(2);
    EXPECT_CALL(*dummyTracker, CopyConstructor()).Times(0);
    EXPECT_CALL(*dummyTracker, Destructor()).Times(0);

    allocator.Construct(ptr, 42);
    allocator.Construct(ptr + 1, 43);
    EXPECT_EQ(ptr[0].value, 42);
    EXPECT_EQ(ptr[1].value, 43);
}

TEST_F(ManagedSharedMemorySegmentAllocatorFixture,
       WHEN_destruct_is_called_THEN_elements_are_destructed)
{
    ManagedSharedMemorySegmentAllocator<Dummy> allocator(mSegment,
                                                         "TestVector");

    Dummy* ptr = allocator.Allocate(2);
    allocator.Construct(ptr, 42);
    allocator.Construct(ptr + 1, 43);

    EXPECT_CALL(*dummyTracker, Destructor()).Times(2);

    allocator.Destruct(ptr);
    allocator.Destruct(ptr + 1);
}

TEST_F(ManagedSharedMemorySegmentAllocatorFixture,
       WHEN_deallocate_is_called_THEN_memory_is_freed)
{
    ManagedSharedMemorySegmentAllocator<Dummy> allocator(mSegment,
                                                         "TestVector");

    Dummy* ptr = allocator.Allocate(2);
    allocator.Construct(ptr, 42);
    allocator.Construct(ptr + 1, 43);

    EXPECT_CALL(*dummyTracker, Destructor()).Times(2);

    allocator.Deallocate(ptr);

    BlockExpectations();
}

}  // namespace Moon::Test
