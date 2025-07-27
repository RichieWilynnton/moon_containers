#include <VectorLib/vector.hpp>
#include <CommonTestLib/dummy.hpp>
#include <CommonTestLib/dummyTracker.hpp>
#include <AllocatorLib/heapAllocator.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace Moon::Test
{
using Dummy = Moon::Common::Test::Dummy;

class VectorFixture : public ::testing::Test
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


TEST_F(VectorFixture, WHEN_vector_is_created_THEN_no_elements_are_constructed)
{
    EXPECT_CALL(*dummyTracker, DefaultConstructor()).Times(0);
    EXPECT_CALL(*dummyTracker, ArgConstructor()).Times(0);
    EXPECT_CALL(*dummyTracker, CopyConstructor()).Times(0);
    EXPECT_CALL(*dummyTracker, Destructor()).Times(0);
    Vector<Dummy> vector;
}

TEST_F(VectorFixture, WHEN_vector_is_created_THEN_it_is_empty)
{
    Vector<Dummy> vector;
    EXPECT_TRUE(vector.Empty());
    EXPECT_EQ(vector.Size(), 0);
}

TEST_F(VectorFixture, WHEN_push_r_value_elements_THEN_elements_are_moved_onto_vector)
{
    Vector<Dummy> vector;

    EXPECT_CALL(*dummyTracker, MoveConstructor())
        .Times(1);  
    EXPECT_CALL(*dummyTracker, Destructor()).Times(1);  
    vector.PushBack(Dummy(42));
    BlockExpectations();
}

TEST_F(VectorFixture,
       WHEN_push_l_value_elements_THEN_elements_are_copied_onto_vector)
{
    Vector<Dummy> vector;

    Dummy d{2};

    EXPECT_CALL(*dummyTracker, CopyConstructor()).Times(1);
    EXPECT_CALL(*dummyTracker, MoveConstructor()).Times(0);
    EXPECT_CALL(*dummyTracker, Destructor()).Times(0);

    vector.PushBack(d);

    BlockExpectations();
}

TEST_F(VectorFixture, WHEN_emplace_is_called_THEN_element_is_constructed_in_place)
{
    Vector<Dummy> vector;

    EXPECT_CALL(*dummyTracker, ArgConstructor()).Times(1);
    EXPECT_CALL(*dummyTracker, CopyConstructor()).Times(0);
    EXPECT_CALL(*dummyTracker, MoveConstructor()).Times(0);
    EXPECT_CALL(*dummyTracker, Destructor()).Times(0);

    vector.EmplaceBack(42);
    BlockExpectations();
}

TEST_F(VectorFixture, WHEN_pop_elements_THEN_vector_size_decreases)
{
    Vector<Dummy> vector;

    vector.PushBack(Dummy(1));
    vector.PushBack(Dummy(2));
    
    EXPECT_CALL(*dummyTracker, Destructor())
        .Times(2);  

    EXPECT_EQ(vector.Size(), 2);
    vector.PopBack();
    EXPECT_EQ(vector.Size(), 1);
    vector.PopBack();
    EXPECT_TRUE(vector.Empty());
}

TEST_F(VectorFixture, WHEN_elements_are_present_THEN_top_returns_last_element)
{
    Vector<Dummy> vector;

    vector.PushBack(Dummy(1));
    EXPECT_EQ(vector.Back().value, 1);

    vector.PushBack(Dummy(2));
    EXPECT_EQ(vector.Back().value, 2);

    vector.PopBack();
    EXPECT_EQ(vector.Back().value, 1);

    vector.PushBack(Dummy(3));
    EXPECT_EQ(vector.Back().value, 3);
}

TEST_F(VectorFixture, WHEN_vector_reallocation_is_triggered_THEN_no_copies_are_made_AND_no_destructors_are_called)
{
    Vector<Dummy> vector;
    const size_t startingCapacity = HeapAllocator<Dummy>::STARTING_CAPACITY;

    for (int i = 0; i < startingCapacity; ++i)
    {
        vector.PushBack(Dummy(i));
    }

    EXPECT_CALL(*dummyTracker, MoveConstructor()).Times(startingCapacity + 1);
    EXPECT_CALL(*dummyTracker, Destructor()).Times(startingCapacity + 1);

    vector.PushBack(Dummy(42));  
    
    BlockExpectations();
}

TEST_F(VectorFixture, WHEN_elements_are_popped_THEN_destructors_are_called)
{
    Vector<Dummy> vector;

    vector.PushBack(Dummy(1));

    EXPECT_CALL(*dummyTracker, Destructor()).Times(1);
    vector.PopBack();

    BlockExpectations();
}

TEST_F(
    VectorFixture,
    WHEN_copy_constructor_is_called_THEN_elements_are_copied_properly)
{
    Vector<Dummy> vector1;
    vector1.PushBack(Dummy(1));
    vector1.PushBack(Dummy(2));

    EXPECT_CALL(*dummyTracker, CopyConstructor()).Times(2);
    Vector<Dummy> vector2(vector1);
    EXPECT_EQ(vector2.Size(), 2);
    EXPECT_EQ(vector2.Back().value, 2);

    BlockExpectations();
}

TEST_F(
    VectorFixture,
    WHEN_copy_constructor_is_called_THEN_original_elements_are_not_modified_on_changes_to_new_vector)
{
    Vector<Dummy> vector1;
    vector1.PushBack(Dummy(1));
    vector1.PushBack(Dummy(2));

    Vector<Dummy> vector2(vector1);

    vector2.Back() = Dummy(3);
    EXPECT_EQ(vector2.Back().value, 3);
    EXPECT_EQ(vector1.Back().value, 2);

    vector2.PopBack();
    EXPECT_EQ(vector2.Size(), 1);
    EXPECT_EQ(vector1.Size(), 2);

    BlockExpectations();
}

TEST_F(VectorFixture, WHEN_copy_assignment_is_called_THEN_elements_are_copied_properly)
{
    Vector<Dummy> vector1;
    vector1.PushBack(Dummy(1));
    vector1.PushBack(Dummy(2));

    Vector<Dummy> vector2;
    EXPECT_CALL(*dummyTracker, CopyConstructor()).Times(2);
    vector2 = vector1;
    EXPECT_EQ(vector2.Size(), 2);
    EXPECT_EQ(vector2.Back().value, 2);
    BlockExpectations();
}


TEST_F(
    VectorFixture,
    WHEN_copy_assignment_is_called_THEN_original_elements_are_not_modified_on_changes_to_new_vector)
{
    Vector<Dummy> vector1;
    vector1.PushBack(Dummy(1));
    vector1.PushBack(Dummy(2));

    Vector<Dummy> vector2 = vector1;

    vector2.Back() = Dummy(3);
    EXPECT_EQ(vector2.Back().value, 3);
    EXPECT_EQ(vector1.Back().value, 2);

    vector2.PopBack();
    EXPECT_EQ(vector2.Size(), 1);
    EXPECT_EQ(vector1.Size(), 2);
    BlockExpectations();
}

TEST_F(VectorFixture, WHEN_move_constructor_is_called_THEN_vector_is_moved_correctly_and_old_vector_is_empty)
{
    Vector<Dummy> vector1;
    vector1.PushBack(Dummy(1));
    vector1.PushBack(Dummy(2));

    EXPECT_CALL(*dummyTracker, MoveConstructor()).Times(0);
    Vector<Dummy> vector2(std::move(vector1));
    EXPECT_EQ(vector2.Size(), 2);
    EXPECT_EQ(vector2.Back().value, 2);
    EXPECT_TRUE(vector1.Empty());  

    BlockExpectations();
}


TEST_F(
    VectorFixture,
    WHEN_move_assignment_is_called_THEN_vector_is_moved_properly_AND_old_vector_is_empty)
{
    Vector<Dummy> vector1;
    vector1.PushBack(Dummy(1));
    vector1.PushBack(Dummy(2));

    EXPECT_CALL(*dummyTracker, MoveConstructor()).Times(0);
    Vector<Dummy> vector2 = std::move(vector1);
    EXPECT_EQ(vector2.Size(), 2);
    EXPECT_EQ(vector2.Back().value, 2);
    EXPECT_TRUE(vector1.Empty());  

    BlockExpectations();
}

TEST_F(VectorFixture, WHEN_vector_is_cleared_THEN_all_elements_are_destructed)
{
    Vector<Dummy> vector;

    vector.PushBack(Dummy(1));
    vector.PushBack(Dummy(2));

    EXPECT_CALL(*dummyTracker, Destructor()).Times(2);
    vector.Clear();
    EXPECT_TRUE(vector.Empty());

    BlockExpectations();
}

TEST_F(VectorFixture, WHEN_at_on_out_of_bounds_index_THEN_exception_is_thrown) {
    Vector<Dummy> vector;
    EXPECT_THROW(vector.At(0), std::out_of_range);
}

// TEST_F(VectorFixture, WHEN_variadic_constructor_is_called_THEN_elements_are_constructed_with_args)
// {
//     EXPECT_CALL(*dummyTracker, ArgConstructor()).Times(3);
//     EXPECT_CALL(*dummyTracker, CopyConstructor()).Times(0);
//     EXPECT_CALL(*dummyTracker, MoveConstructor()).Times(0);
//     EXPECT_CALL(*dummyTracker, Destructor()).Times(0);
//
//     Vector<Dummy> vector(1, 2, 3);
//
//     EXPECT_EQ(vector.Size(), 3);
//     EXPECT_EQ(vector[0].value, 1);
//     EXPECT_EQ(vector[1].value, 2);
//     EXPECT_EQ(vector[2].value, 3);
//
//     BlockExpectations();
// }

TEST_F(VectorFixture, WHEN_variadic_constructor_is_called_with_no_args_THEN_vector_is_empty)
{
    Vector<Dummy> vector;
    EXPECT_TRUE(vector.Empty());
    EXPECT_EQ(vector.Size(), 0);
}

}
