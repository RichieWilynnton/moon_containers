#define BOOST_TEST_MODULE StackTests
#include <StackLib/stack.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>

namespace Moon::Test
{

struct Dummy
{
    int value;

    static int destructorCount;
    static int noArgConstructorCount;
    static int paramConstructorCount;
    static int copyConstructorCount;
    static int moveConstructorCount;
    static int copyAssignmentCount;
    static int moveAssignmentCount;

    Dummy() : value(0)
    {
        noArgConstructorCount++;
    }
    Dummy(int v) : value(v)
    {
        paramConstructorCount++;
    }
    Dummy(const Dummy& other) : value(other.value)
    {
        copyConstructorCount++;
    }
    Dummy(Dummy&& other)
    {
        value = other.value;
        other.value = 0;
        moveConstructorCount++;
    }
    Dummy& operator=(const Dummy& other)
    {
        value = other.value;
        copyAssignmentCount++;
        return *this;
    }
    Dummy& operator=(Dummy&& other)
    {
        value = other.value;
        other.value = 0;
        moveAssignmentCount++;
        return *this;
    }
    ~Dummy()
    {
        ++destructorCount;
    }
};

// Initialize static members
int Dummy::destructorCount = 0;
int Dummy::noArgConstructorCount = 0;
int Dummy::paramConstructorCount = 0;
int Dummy::copyConstructorCount = 0;
int Dummy::moveConstructorCount = 0;
int Dummy::copyAssignmentCount = 0;
int Dummy::moveAssignmentCount = 0;

struct StackFixture
{
    StackFixture()
    {
        Dummy::destructorCount = 0;
        Dummy::noArgConstructorCount = 0;
        Dummy::paramConstructorCount = 0;
        Dummy::copyConstructorCount = 0;
        Dummy::moveConstructorCount = 0;
        Dummy::copyAssignmentCount = 0;
        Dummy::moveAssignmentCount = 0;
    }
    ~StackFixture() {}
};

BOOST_FIXTURE_TEST_SUITE(StackTestSuite, StackFixture)

BOOST_AUTO_TEST_CASE(WHEN_stack_is_created_THEN_no_elements_are_constructed)
{
    Stack<Dummy> stack;
    BOOST_CHECK_EQUAL(Dummy::noArgConstructorCount, 0);
    BOOST_CHECK_EQUAL(Dummy::paramConstructorCount, 0);
    BOOST_CHECK_EQUAL(Dummy::copyConstructorCount, 0);
    BOOST_CHECK_EQUAL(Dummy::moveConstructorCount, 0);
}

BOOST_AUTO_TEST_CASE(WHEN_stack_is_created_THEN_it_is_empty)
{
    Stack<Dummy> stack;
    BOOST_CHECK(stack.Empty());
    BOOST_CHECK_EQUAL(stack.Size(), 0);
}

BOOST_AUTO_TEST_CASE(WHEN_push_elements_THEN_elements_are_copied_onto_stack)
{
    Stack<Dummy> stack;
    stack.Push(42);
    BOOST_CHECK_EQUAL(Dummy::copyAssignmentCount, 1);
    BOOST_CHECK_EQUAL(Dummy::destructorCount, 1);
    BOOST_CHECK_EQUAL(Dummy::paramConstructorCount, 1);
}

BOOST_AUTO_TEST_CASE(
    WHEN_emplace_is_called_THEN_element_is_constructed_in_place)
{
    Stack<Dummy> stack;
    stack.Emplace(42);
    BOOST_CHECK_EQUAL(Dummy::copyAssignmentCount, 0);
    BOOST_CHECK_EQUAL(Dummy::destructorCount, 0);
    BOOST_CHECK_EQUAL(Dummy::paramConstructorCount, 1);
}

BOOST_AUTO_TEST_CASE(WHEN_pop_elements_THEN_stack_size_decreases)
{
    Stack<Dummy> stack;
    stack.Push(Dummy(1));
    stack.Push(Dummy(2));
    BOOST_CHECK_EQUAL(stack.Size(), 2);
    BOOST_CHECK_EQUAL(stack.Top().value, 2);

    stack.Pop();
    BOOST_CHECK_EQUAL(stack.Size(), 1);
    BOOST_CHECK_EQUAL(stack.Top().value, 1);

    stack.Pop();
    BOOST_CHECK(stack.Empty());
    BOOST_CHECK_THROW(stack.Top(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(WHEN_pop_from_empty_stack_THEN_exception_is_thrown)
{
    Stack<Dummy> stack;
    BOOST_CHECK_THROW(stack.Pop(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(WHEN_elements_are_present_THEN_top_returns_last_element)
{
    Stack<Dummy> stack;

    stack.Push(Dummy(1));
    BOOST_CHECK_EQUAL(stack.Top().value, 1);

    stack.Push(Dummy(2));
    BOOST_CHECK_EQUAL(stack.Top().value, 2);

    stack.Pop();
    BOOST_CHECK_EQUAL(stack.Top().value, 1);

    stack.Push(Dummy(3));
    BOOST_CHECK_EQUAL(stack.Top().value, 3);
}

BOOST_AUTO_TEST_CASE(WHEN_top_on_empty_stack_THEN_exception_is_thrown)
{
    Stack<Dummy> stack;
    BOOST_CHECK_THROW(stack.Top(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(
    WHEN_stack_is_empty_THEN_implicit_bool_conversion_returns_false_ELSE_true)
{
    Stack<Dummy> stack;
    BOOST_CHECK(!stack);

    stack.Push(Dummy(1));
    BOOST_CHECK(stack);

    stack.Pop();
    BOOST_CHECK(!stack);
}

BOOST_AUTO_TEST_CASE(WHEN_stack_resize_is_triggered_THEN_no_copies_are_made)
{
    Stack<Dummy> stack;

    for (int i = 0; i < 15; ++i)
    {
        stack.Push(Dummy(i));
        BOOST_CHECK_EQUAL(Dummy::copyAssignmentCount, i + 1);
        BOOST_CHECK_EQUAL(Dummy::destructorCount, i + 1);
        if (i >= Stack<Dummy>::STARTING_CAPACITY)
            BOOST_CHECK_EQUAL(Dummy::moveAssignmentCount,
                              Stack<Dummy>::STARTING_CAPACITY);
    }
    BOOST_CHECK_EQUAL(stack.Size(), 15);
    BOOST_CHECK_EQUAL(stack.Top().value, 14);

    for (int i = 0; i < 15; ++i)
    {
        stack.Pop();
    }
    BOOST_CHECK(stack.Empty());
    BOOST_CHECK_EQUAL(Dummy::destructorCount, 30);
}

BOOST_AUTO_TEST_CASE(WHEN_elements_are_popped_THEN_destructors_are_called)
{
    Stack<Dummy> stack;
    stack.Push(Dummy(1));
    stack.Push(Dummy(2));
    BOOST_CHECK_EQUAL(Dummy::destructorCount, 2);

    stack.Pop();
    BOOST_CHECK_EQUAL(Dummy::destructorCount, 3);
    BOOST_CHECK_EQUAL(stack.Size(), 1);

    stack.Pop();
    BOOST_CHECK_EQUAL(Dummy::destructorCount, 4);
}
BOOST_AUTO_TEST_CASE(
    WHEN_copy_constructor_is_called_THEN_original_elements_are_not_modified_on_modifications_to_new_stack)
{
    Stack<Dummy> stack1;
    stack1.Push(Dummy(1));
    BOOST_CHECK_EQUAL(Dummy::copyAssignmentCount, 1);
    stack1.Push(Dummy(2));
    BOOST_CHECK_EQUAL(Dummy::copyAssignmentCount, 2);

    Stack<Dummy> stack2(stack1);
    BOOST_CHECK_EQUAL(Dummy::copyAssignmentCount, 4);
    BOOST_CHECK_EQUAL(stack2.Size(), 2);
    BOOST_CHECK_EQUAL(stack2.Top().value, 2);

    stack2.Top() = Dummy(3);
    BOOST_CHECK_EQUAL(stack2.Top().value, 3);
    BOOST_CHECK_EQUAL(stack1.Top().value, 2);

    stack2.Pop();
    BOOST_CHECK_EQUAL(stack2.Size(), 1);
    BOOST_CHECK_EQUAL(stack1.Size(), 2);
}

BOOST_AUTO_TEST_CASE(
    WHEN_move_constructor_is_called_THEN_stack_is_moved_correctly)
{
    Stack<Dummy> stack1;
    stack1.Push(Dummy(1));
    stack1.Push(Dummy(2));
    BOOST_CHECK_EQUAL(Dummy::moveAssignmentCount, 0);
    BOOST_CHECK_EQUAL(Dummy::copyAssignmentCount, 2);

    Stack<Dummy> stack2(std::move(stack1));
    BOOST_CHECK_EQUAL(Dummy::moveAssignmentCount, 0);
    BOOST_CHECK_EQUAL(stack2.Size(), 2);
    BOOST_CHECK_EQUAL(stack2.Top().value, 2);
}

BOOST_AUTO_TEST_CASE(WHEN_copy_assignment_is_called_THEN_elements_are_copied)
{
    Stack<Dummy> stack1;
    stack1.Push(Dummy(1));
    BOOST_CHECK_EQUAL(Dummy::copyAssignmentCount, 1);
    stack1.Push(Dummy(2));
    BOOST_CHECK_EQUAL(Dummy::copyAssignmentCount, 2);

    Stack<Dummy> stack2;
    stack2 = stack1;
    BOOST_CHECK_EQUAL(Dummy::copyAssignmentCount, 4);
    BOOST_CHECK_EQUAL(stack2.Size(), 2);
    BOOST_CHECK_EQUAL(stack2.Top().value, 2);
}

BOOST_AUTO_TEST_CASE(
    WHEN_move_assignment_is_called_THEN_old_stack_is_cleared_and_new_stack_is_moved)
{
    Stack<Dummy> stack1;
    stack1.Push(Dummy(1));
    stack1.Push(Dummy(2));
    BOOST_CHECK_EQUAL(Dummy::moveAssignmentCount, 0);
    BOOST_CHECK_EQUAL(Dummy::copyAssignmentCount, 2);
    BOOST_CHECK_EQUAL(Dummy::destructorCount, 2);

    Stack<Dummy> stack2;
    stack2.Push(Dummy(3));
    BOOST_CHECK_EQUAL(Dummy::copyAssignmentCount, 3);
    BOOST_CHECK_EQUAL(Dummy::destructorCount, 3);

    stack2 = std::move(stack1);
    BOOST_CHECK_EQUAL(Dummy::moveAssignmentCount, 0);
    BOOST_CHECK_EQUAL(Dummy::copyAssignmentCount, 3);
    BOOST_CHECK_EQUAL(Dummy::destructorCount, 4);

    BOOST_CHECK_EQUAL(stack2.Size(), 2);
    BOOST_CHECK_EQUAL(stack2.Top().value, 2);
}

BOOST_AUTO_TEST_SUITE_END()
}  // namespace Moon::Test
