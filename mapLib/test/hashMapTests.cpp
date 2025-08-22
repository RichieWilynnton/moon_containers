#include <CommonTestLib/dummy.hpp>
#include <CommonTestLib/dummyTracker.hpp>
#include <AllocatorLib/debugAllocator.hpp>
#include <MapLib/hashMap.hpp>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace Moon::Test
{
using Dummy = Moon::Common::Test::Dummy;

class MapFixture : public ::testing::Test
{
    void SetUp() override
    {
        ::testing::GTEST_FLAG(throw_on_failure) = true;
        ::testing::InitGoogleMock();
        dummyTracker = new DummyTracker();
        Dummy::tracker = dummyTracker;
    }

    void TearDown() override
    {
        EXPECT_NO_THROW(DebugAllocator<Dummy>::ReportLeaks());
        delete dummyTracker;
        Dummy::tracker = nullptr;
    }

    void BlockExpectations()
    {
        ::testing::Mock::VerifyAndClearExpectations(dummyTracker);
    }

    DummyTracker* dummyTracker;
};

TEST_F(MapFixture, WHEN_map_is_created_THEN_no_elements_are_constructed)
{
    EXPECT_CALL(*dummyTracker, DefaultConstructor()).Times(0);
    EXPECT_CALL(*dummyTracker, ArgConstructor()).Times(0);
    EXPECT_CALL(*dummyTracker, CopyConstructor()).Times(0);
    EXPECT_CALL(*dummyTracker, Destructor()).Times(0);
    
    HashMap<int, Dummy> map;
}
}  // namespace Moon::Test
