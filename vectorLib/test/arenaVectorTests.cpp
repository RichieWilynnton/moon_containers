#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <CommonTestLib/dummy.hpp>
#include <VectorLib/vector.hpp>
#include <AllocatorLib/arenaAllocator.hpp>

namespace Moon::Test
{
using Dummy = Moon::Common::Test::Dummy;

class ArenaVectorFixture : public ::testing::Test
{
   protected:
    template <typename T>
    using ArenaVector = Vector<T, ArenaAllocator<T>>;

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

TEST_F(ArenaVectorFixture, WHEN_element_is_pushed_back_THEN_size_increases_and_element_is_accessible)
{
    Arena arena(1024);
    ArenaAllocator<Dummy> allocator(&arena);
    ArenaVector<Dummy> vec(allocator);

    vec.PushBack(Dummy(42));

    EXPECT_EQ(vec.Size(), 1);
    EXPECT_EQ(vec[0].value, 42);
    BlockExpectations();
}

TEST_F(ArenaVectorFixture, WHEN_multiple_elements_are_pushed_back_THEN_elements_are_stored_in_order)
{
    Arena arena(1024);
    ArenaAllocator<Dummy> allocator(&arena);
    ArenaVector<Dummy> vec(allocator);

    vec.PushBack(Dummy(10));
    vec.PushBack(Dummy(20));
    vec.PushBack(Dummy(30));

    EXPECT_EQ(vec.Size(), 3);
    EXPECT_EQ(vec[0].value, 10);
    EXPECT_EQ(vec[1].value, 20);
    EXPECT_EQ(vec[2].value, 30);
    BlockExpectations();
}

TEST_F(ArenaVectorFixture, WHEN_element_is_popped_back_THEN_size_decreases_and_element_is_removed)
{
    Arena arena(1024);
    ArenaAllocator<Dummy> allocator(&arena);
    ArenaVector<Dummy> vec(allocator);

    vec.PushBack(Dummy(10));
    vec.PushBack(Dummy(20));
    vec.PopBack();

    EXPECT_EQ(vec.Size(), 1);
    EXPECT_EQ(vec[0].value, 10);
    BlockExpectations();
}

TEST_F(ArenaVectorFixture, WHEN_vector_is_cleared_THEN_size_becomes_zero)
{
    Arena arena(1024);
    ArenaAllocator<Dummy> allocator(&arena);
    ArenaVector<Dummy> vec(allocator);

    vec.PushBack(Dummy(10));
    vec.PushBack(Dummy(20));
    vec.Clear();

    EXPECT_EQ(vec.Size(), 0);
    BlockExpectations();
}

TEST_F(ArenaVectorFixture, WHEN_elements_are_accessed_via_iterator_THEN_elements_are_traversed_in_order)
{
    Arena arena(1024);
    ArenaAllocator<Dummy> allocator(&arena);
    ArenaVector<Dummy> vec(allocator);

    vec.PushBack(Dummy(10));
    vec.PushBack(Dummy(20));
    vec.PushBack(Dummy(30));

    int expected_values[] = {10, 20, 30};
    int index = 0;
    for (auto it = vec.Begin(); it != vec.End(); ++it)
    {
        EXPECT_EQ(it->value, expected_values[index++]);
    }
    EXPECT_EQ(index, 3);
    BlockExpectations();
}
//
// TEST_F(ArenaVectorFixture, WHEN_vector_is_resized_smaller_THEN_excess_elements_are_removed)
// {
//     Arena arena(1024);
//     ArenaAllocator<Dummy> allocator(&arena);
//     ArenaVector<Dummy> vec(allocator);
//
//     vec.PushBack(Dummy(10));
//     vec.PushBack(Dummy(20));
//     vec.PushBack(Dummy(30));
//     vec.Resize(1);
//
//     EXPECT_EQ(vec.Size(), 1);
//     EXPECT_EQ(vec[0].value, 10);
//     BlockExpectations();
// }
//
// TEST_F(ArenaVectorFixture, WHEN_vector_is_resized_larger_THEN_new_elements_are_default_initialized)
// {
//     Arena arena(1024);
//     ArenaAllocator<Dummy> allocator(&arena);
//     ArenaVector<Dummy> vec(allocator);
//
//     vec.PushBack(Dummy(10));
//     vec.Resize(3);
//
//     EXPECT_EQ(vec.Size(), 3);
//     EXPECT_EQ(vec[0].value, 10);
//     EXPECT_EQ(vec[1].value, 0); // Assuming Dummy default constructor sets value to 0
//     EXPECT_EQ(vec[2].value, 0);
//     BlockExpectations();
// }
//
// TEST_F(ArenaVectorFixture, WHEN_vector_grows_beyond_capacity_THEN_elements_remain_intact_after_reallocation)
// {
//     Arena arena(1024);
//     ArenaAllocator<Dummy> allocator(&arena);
//     ArenaVector<Dummy> vec(allocator);
//
//     // Assuming initial capacity is small (e.g., 4)
//     size_t initial_capacity = allocator.GetStartingCapacity();
//     for (int i = 0; i < static_cast<int>(initial_capacity); ++i)
//     {
//         vec.PushBack(Dummy(i + 1));
//     }
//
//     // Trigger reallocation
//     vec.PushBack(Dummy(100));
//
//     EXPECT_GT(vec.Capacity(), initial_capacity);
//     EXPECT_EQ(vec.Size(), initial_capacity + 1);
//     for (int i = 0; i < static_cast<int>(initial_capacity); ++i)
//     {
//         EXPECT_EQ(vec[i].value, i + 1);
//     }
//     EXPECT_EQ(vec[initial_capacity].value, 100);
//     BlockExpectations();
// }
//
// TEST_F(ArenaVectorFixture, WHEN_element_is_inserted_at_index_THEN_elements_are_shifted_and_size_increases)
// {
//     Arena arena(1024);
//     ArenaAllocator<Dummy> allocator(&arena);
//     ArenaVector<Dummy> vec(allocator);
//
//     vec.PushBack(Dummy(10));
//     vec.PushBack(Dummy(30));
//     vec.Insert(1, Dummy(20));
//
//     EXPECT_EQ(vec.Size(), 3);
//     EXPECT_EQ(vec[0].value, 10);
//     EXPECT_EQ(vec[1].value, 20);
//     EXPECT_EQ(vec[2].value, 30);
//     BlockExpectations();
// }
//
// TEST_F(ArenaVectorFixture, WHEN_element_is_erased_at_index_THEN_elements_are_shifted_and_size_decreases)
// {
//     Arena arena(1024);
//     ArenaAllocator<Dummy> allocator(&arena);
//     ArenaVector<Dummy> vec(allocator);
//
//     vec.PushBack(Dummy(10));
//     vec.PushBack(Dummy(20));
//     vec.PushBack(Dummy(30));
//     vec.Erase(1);
//
//     EXPECT_EQ(vec.Size(), 2);
//     EXPECT_EQ(vec[0].value, 10);
//     EXPECT_EQ(vec[1].value, 30);
//     BlockExpectations();
// }
//
// TEST_F(ArenaVectorFixture, WHEN_vector_is_copy_constructed_THEN_elements_are_copied)
// {
//     Arena arena(1024);
//     ArenaAllocator<Dummy> allocator(&arena);
//     ArenaVector<Dummy> vec(allocator);
//
//     vec.PushBack(Dummy(10));
//     vec.PushBack(Dummy(20));
//
//     Arena arena2(1024);
//     ArenaAllocator<Dummy> allocator2(arena2);
//     ArenaVector<Dummy> vec2(vec, allocator2);
//
//     EXPECT_EQ(vec2.Size(), 2);
//     EXPECT_EQ(vec2[0].value, 10);
//     EXPECT_EQ(vec2[1].value, 20);
//     EXPECT_EQ(vec.Size(), 2); // Original vector unchanged
//     EXPECT_EQ(vec[0].value, 10);
//     EXPECT_EQ(vec[1].value, 20);
//     BlockExpectations();
// }
//
// TEST_F(ArenaVectorFixture, WHEN_vector_is_move_constructed_THEN_elements_are_transferred)
// {
//     Arena arena(1024);
//     ArenaAllocator<Dummy> allocator(&arena);
//     ArenaVector<Dummy> vec(allocator);
//
//     vec.PushBack(Dummy(10));
//     vec.PushBack(Dummy(20));
//
//     Arena arena2(1024);
//     ArenaAllocator<Dummy> allocator2(arena2);
//     ArenaVector<Dummy> vec2(std::move(vec), allocator2);
//
//     EXPECT_EQ(vec2.Size(), 2);
//     EXPECT_EQ(vec2[0].value, 10);
//     EXPECT_EQ(vec2[1].value, 20);
//     EXPECT_EQ(vec.Size(), 0);
//     BlockExpectations();
// }
//
}  // namespace Moon::Test
