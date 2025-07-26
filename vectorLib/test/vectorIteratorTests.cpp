#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <CommonTestLib/dummy.hpp>
#include <CommonTestLib/dummyTracker.hpp>
#include <VectorLib/vector.hpp>
#include <VectorLib/vectorIterator.hpp>

namespace Moon::Test
{
using Dummy = Moon::Common::Test::Dummy;

class VectorIteratorFixture : public ::testing::Test
{
   protected:
    void SetUp() override
    {
        ::testing::GTEST_FLAG(throw_on_failure) = true;
        ::testing::InitGoogleMock();
        dummyTracker = new DummyTracker();
        Dummy::tracker = dummyTracker;
        
        vector.PushBack(Dummy(1));
        vector.PushBack(Dummy(2));
        vector.PushBack(Dummy(3));
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
    Vector<Dummy> vector;
};

TEST_F(VectorIteratorFixture, 
       WHEN_prefix_increment_is_called_THEN_iterator_moves_to_next_element)
{
    auto it = vector.begin();
    EXPECT_EQ((++it)->value, 2);
    EXPECT_EQ((++it)->value, 3);
}

TEST_F(VectorIteratorFixture, 
       WHEN_postfix_increment_is_called_THEN_iterator_moves_after_evaluation)
{
    auto it = vector.begin();
    EXPECT_EQ((it++)->value, 1);
    EXPECT_EQ(it->value, 2);
}

TEST_F(VectorIteratorFixture, 
       WHEN_prefix_decrement_is_called_THEN_iterator_moves_to_previous_element)
{
    auto it = vector.end();
    --it;
    EXPECT_EQ((--it)->value, 2);
}

TEST_F(VectorIteratorFixture, 
       WHEN_postfix_decrement_is_called_THEN_iterator_moves_after_evaluation)
{
    auto it = vector.end();
    --it;
    EXPECT_EQ((it--)->value, 3);
    EXPECT_EQ(it->value, 2);
}

TEST_F(VectorIteratorFixture, 
       WHEN_adding_offset_THEN_iterator_moves_forward_by_that_amount)
{
    auto it = vector.begin();
    auto result = it + 2;
    EXPECT_EQ(result->value, 3);
}

TEST_F(VectorIteratorFixture, 
       WHEN_subtracting_offset_THEN_iterator_moves_backward_by_that_amount)
{
    auto it = vector.end();
    auto result = it - 2;
    EXPECT_EQ(result->value, 2);
}

TEST_F(VectorIteratorFixture, 
       WHEN_comparing_equal_iterators_THEN_they_are_considered_equal)
{
    auto it1 = vector.begin();
    auto it2 = vector.begin();
    EXPECT_TRUE(it1 == it2);
    
    ++it2;
    EXPECT_FALSE(it1 == it2);
}

TEST_F(VectorIteratorFixture, 
       WHEN_comparing_unequal_iterators_THEN_they_are_considered_unequal)
{
    auto it1 = vector.begin();
    auto it2 = vector.begin();
    EXPECT_FALSE(it1 != it2);
    
    ++it2;
    EXPECT_TRUE(it1 != it2);
}

TEST_F(VectorIteratorFixture, 
       WHEN_comparing_greater_position_THEN_returns_true_for_later_position)
{
    auto begin = vector.begin();
    auto end = vector.end();
    EXPECT_TRUE(end > begin);
    EXPECT_FALSE(begin > end);
}

TEST_F(VectorIteratorFixture, 
       WHEN_comparing_less_position_THEN_returns_true_for_earlier_position)
{
    auto begin = vector.begin();
    auto end = vector.end();
    EXPECT_TRUE(begin < end);
    EXPECT_FALSE(end < begin);
}

TEST_F(VectorIteratorFixture, 
       WHEN_comparing_greater_or_equal_position_THEN_returns_true_for_later_or_same_position)
{
    auto begin = vector.begin();
    auto end = vector.end();
    EXPECT_TRUE(end >= begin);
    EXPECT_TRUE(begin >= begin);
    EXPECT_FALSE(begin >= end);
}

TEST_F(VectorIteratorFixture, 
       WHEN_comparing_less_or_equal_position_THEN_returns_true_for_earlier_or_same_position)
{
    auto begin = vector.begin();
    auto end = vector.end();
    EXPECT_TRUE(begin <= end);
    EXPECT_TRUE(begin <= begin);
    EXPECT_FALSE(end <= begin);
}

TEST_F(VectorIteratorFixture, 
       WHEN_dereferencing_iterator_THEN_returns_reference_to_current_element)
{
    auto it = vector.begin();
    EXPECT_EQ((*it).value, 1);
    ++it;
    EXPECT_EQ((*it).value, 2);
}

TEST_F(VectorIteratorFixture, 
       WHEN_using_arrow_operator_THEN_provides_access_to_current_element_members)
{
    auto it = vector.begin();
    EXPECT_EQ(it->value, 1);
    ++it;
    EXPECT_EQ(it->value, 2);
}

TEST_F(VectorIteratorFixture,
       WHEN_using_range_based_for_loop_THEN_it_iterates_over_all_elements)
{
    std::vector<int> values;
    for (const auto& dummy : vector) {
        values.push_back(dummy.value);
    }
    
    EXPECT_THAT(values, ::testing::ElementsAre(1, 2, 3));
}

TEST_F(VectorIteratorFixture,
       WHEN_modifying_elements_in_range_based_for_loop_THEN_changes_are_reflected)
{
    for (auto& dummy : vector) {
        dummy.value *= 2;
    }
    
    std::vector<int> values;
    for (const auto& dummy : vector) {
        values.push_back(dummy.value);
    }
    
    EXPECT_THAT(values, ::testing::ElementsAre(2, 4, 6));
}

TEST_F(VectorIteratorFixture,
       WHEN_using_range_based_for_loop_on_empty_vector_THEN_no_iterations_occur)
{
    Vector<Dummy> emptyVector;
    bool loopEntered = false;
    
    for (const auto& dummy : emptyVector) {
        loopEntered = true;
        (void)dummy; 
    }
    
    EXPECT_FALSE(loopEntered);
}

TEST_F(VectorIteratorFixture,
       WHEN_using_range_based_for_loop_with_const_vector_THEN_it_iterates_correctly)
{
    const Vector<Dummy> constVector = vector;
    std::vector<int> values;
    
    for (const auto& dummy : constVector) {
        values.push_back(dummy.value);
    }
    
    EXPECT_THAT(values, ::testing::ElementsAre(1, 2, 3));
}
}  // namespace Moon::Test
