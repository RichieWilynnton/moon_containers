#include <StackLib/stack.hpp>
#include <CommonTestLib/dummy.hpp>
#include <CommonTestLib/dummyTracker.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace Moon::Test
{
using Dummy = Moon::Common::Test::Dummy;

class StackFixture : public ::testing::Test
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

TEST_F(StackFixture, WHEN_stack_is_created_THEN_no_elements_are_constructed)
{
    EXPECT_CALL(*dummyTracker, DefaultConstructor()).Times(0);
    EXPECT_CALL(*dummyTracker, ArgConstructor()).Times(0);
    EXPECT_CALL(*dummyTracker, CopyConstructor()).Times(0);
    EXPECT_CALL(*dummyTracker, Destructor()).Times(0);
    Stack<Dummy> stack;
}

TEST_F(StackFixture, WHEN_stack_is_created_THEN_it_is_empty)
{
    Stack<Dummy> stack;
    EXPECT_TRUE(stack.Empty());
    EXPECT_EQ(stack.Size(), 0);
}

TEST_F(StackFixture, WHEN_push_r_value_elements_THEN_elements_are_moved_onto_stack)
{
    Stack<Dummy> stack;

    EXPECT_CALL(*dummyTracker, MoveConstructor())
        .Times(1);  
    EXPECT_CALL(*dummyTracker, Destructor()).Times(1);  
    stack.Push(Dummy(42));
    BlockExpectations();
}

TEST_F(StackFixture,
       WHEN_push_l_value_elements_THEN_elements_are_copied_onto_stack)
{
    Stack<Dummy> stack;

    Dummy d{2};

    EXPECT_CALL(*dummyTracker, CopyConstructor()).Times(1);
    EXPECT_CALL(*dummyTracker, MoveConstructor()).Times(0);
    EXPECT_CALL(*dummyTracker, Destructor()).Times(0);

    stack.Push(d);

    BlockExpectations();
}

TEST_F(StackFixture, WHEN_emplace_is_called_THEN_element_is_constructed_in_place)
{
    Stack<Dummy> stack;

    EXPECT_CALL(*dummyTracker, ArgConstructor()).Times(1);
    EXPECT_CALL(*dummyTracker, CopyConstructor()).Times(0);
    EXPECT_CALL(*dummyTracker, MoveConstructor()).Times(0);
    EXPECT_CALL(*dummyTracker, Destructor()).Times(0);

    stack.Emplace(42);
    BlockExpectations();
}

TEST_F(StackFixture, WHEN_pop_elements_THEN_stack_size_decreases)
{
    Stack<Dummy> stack;

    
    EXPECT_CALL(*dummyTracker, ArgConstructor()).Times(2);
    EXPECT_CALL(*dummyTracker, MoveConstructor()).Times(2);
    EXPECT_CALL(*dummyTracker, Destructor()).Times(2);  

    stack.Push(Dummy(1));
    stack.Push(Dummy(2));

    
    EXPECT_CALL(*dummyTracker, Destructor())
        .Times(2);  

    EXPECT_EQ(stack.Size(), 2);
    stack.Pop();
    EXPECT_EQ(stack.Size(), 1);
    stack.Pop();
    EXPECT_TRUE(stack.Empty());
}

TEST_F(StackFixture, WHEN_top_on_empty_stack_THEN_exception_is_thrown)
{
    Stack<Dummy> stack;
    EXPECT_THROW(stack.Top(), std::runtime_error);
}

TEST_F(StackFixture, WHEN_elements_are_present_THEN_top_returns_last_element)
{
    Stack<Dummy> stack;

    stack.Push(Dummy(1));
    EXPECT_EQ(stack.Top().value, 1);

    stack.Push(Dummy(2));
    EXPECT_EQ(stack.Top().value, 2);

    stack.Pop();
    EXPECT_EQ(stack.Top().value, 1);

    stack.Push(Dummy(3));
    EXPECT_EQ(stack.Top().value, 3);
}

TEST_F(
    StackFixture,
    WHEN_stack_is_empty_THEN_implicit_bool_conversion_returns_false_ELSE_true)
{
    Stack<Dummy> stack;
    EXPECT_FALSE(stack);

    stack.Push(Dummy(1));
    EXPECT_TRUE(stack);

    stack.Pop();
    EXPECT_FALSE(stack);
}

TEST_F(StackFixture, WHEN_stack_resize_is_triggered_THEN_no_copies_are_made_AND_no_destructors_are_called)
{
    Stack<Dummy> stack;

    for (int i = 0; i < Stack<Dummy>::STARTING_CAPACITY; ++i)
    {
        stack.Push(Dummy(i));
    }

    EXPECT_CALL(*dummyTracker, MoveConstructor()).Times(Stack<Dummy>::STARTING_CAPACITY + 1);
    EXPECT_CALL(*dummyTracker, Destructor()).Times(1);

    stack.Push(Dummy(42));  
    
    BlockExpectations();
}

TEST_F(StackFixture, WHEN_elements_are_popped_THEN_destructors_are_called)
{
    Stack<Dummy> stack;

    stack.Push(Dummy(1));

    EXPECT_CALL(*dummyTracker, Destructor()).Times(1);
    stack.Pop();

    BlockExpectations();
}

TEST_F(
    StackFixture,
    WHEN_copy_constructor_is_called_THEN_elements_are_copied_properly)
{
    Stack<Dummy> stack1;
    stack1.Push(Dummy(1));
    stack1.Push(Dummy(2));

    EXPECT_CALL(*dummyTracker, CopyAssignment()).Times(2);
    Stack<Dummy> stack2(stack1);
    EXPECT_EQ(stack2.Size(), 2);
    EXPECT_EQ(stack2.Top().value, 2);

    BlockExpectations();
}

TEST_F(
    StackFixture,
    WHEN_copy_constructor_is_called_THEN_original_elements_are_not_modified_on_changes_to_new_stack)
{
    Stack<Dummy> stack1;
    stack1.Push(Dummy(1));
    stack1.Push(Dummy(2));

    Stack<Dummy> stack2(stack1);

    stack2.Top() = Dummy(3);
    EXPECT_EQ(stack2.Top().value, 3);
    EXPECT_EQ(stack1.Top().value, 2);

    stack2.Pop();
    EXPECT_EQ(stack2.Size(), 1);
    EXPECT_EQ(stack1.Size(), 2);

    BlockExpectations();
}

TEST_F(StackFixture, WHEN_copy_assignment_is_called_THEN_elements_are_copied_properly)
{
    Stack<Dummy> stack1;
    stack1.Push(Dummy(1));
    stack1.Push(Dummy(2));

    Stack<Dummy> stack2;
    EXPECT_CALL(*dummyTracker, CopyConstructor()).Times(2);
    stack2 = stack1;
    EXPECT_EQ(stack2.Size(), 2);
    EXPECT_EQ(stack2.Top().value, 2);
    BlockExpectations();
}


TEST_F(
    StackFixture,
    WHEN_copy_assignment_is_called_THEN_original_elements_are_not_modified_on_changes_to_new_stack)
{
    Stack<Dummy> stack1;
    stack1.Push(Dummy(1));
    stack1.Push(Dummy(2));

    Stack<Dummy> stack2 = stack1;

    stack2.Top() = Dummy(3);
    EXPECT_EQ(stack2.Top().value, 3);
    EXPECT_EQ(stack1.Top().value, 2);

    stack2.Pop();
    EXPECT_EQ(stack2.Size(), 1);
    EXPECT_EQ(stack1.Size(), 2);
    BlockExpectations();
}

TEST_F(StackFixture, WHEN_move_constructor_is_called_THEN_stack_is_moved_correctly_and_old_stack_is_empty)
{
    Stack<Dummy> stack1;
    stack1.Push(Dummy(1));
    stack1.Push(Dummy(2));

    EXPECT_CALL(*dummyTracker, MoveConstructor()).Times(0);
    Stack<Dummy> stack2(std::move(stack1));
    EXPECT_EQ(stack2.Size(), 2);
    EXPECT_EQ(stack2.Top().value, 2);
    EXPECT_TRUE(stack1.Empty());  

    BlockExpectations();
}


TEST_F(
    StackFixture,
    WHEN_move_assignment_is_called_THEN_stack_is_moved_properly_AND_old_stack_is_empty)
{
    Stack<Dummy> stack1;
    stack1.Push(Dummy(1));
    stack1.Push(Dummy(2));

    EXPECT_CALL(*dummyTracker, MoveConstructor()).Times(0);
    Stack<Dummy> stack2 = std::move(stack1);
    EXPECT_EQ(stack2.Size(), 2);
    EXPECT_EQ(stack2.Top().value, 2);
    EXPECT_TRUE(stack1.Empty());  

    BlockExpectations();
}

}  



