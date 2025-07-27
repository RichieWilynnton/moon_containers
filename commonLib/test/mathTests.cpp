#include <CommonLib/math.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace Moon::Test {

TEST(MathTest, NextPowerOfTwo_ReturnsCorrectResult) {
    EXPECT_EQ(Util::Math::NextPowerOfTwo(0), 1);
    EXPECT_EQ(Util::Math::NextPowerOfTwo(1), 1);
    EXPECT_EQ(Util::Math::NextPowerOfTwo(2), 2);
    EXPECT_EQ(Util::Math::NextPowerOfTwo(3), 4);
    EXPECT_EQ(Util::Math::NextPowerOfTwo(5), 8);
    EXPECT_EQ(Util::Math::NextPowerOfTwo(16), 16);
    EXPECT_EQ(Util::Math::NextPowerOfTwo(17), 32);
    EXPECT_EQ(Util::Math::NextPowerOfTwo(1023), 1024);
    EXPECT_EQ(Util::Math::NextPowerOfTwo(1024), 1024);
    EXPECT_EQ(Util::Math::NextPowerOfTwo(1025), 2048);
    EXPECT_EQ(Util::Math::NextPowerOfTwo(65535), 65536);
}

TEST(MathTest, PreviousPowerOfTwo_ReturnsCorrectResult) {
    EXPECT_EQ(Util::Math::PreviousPowerOfTwo(0), 0);
    EXPECT_EQ(Util::Math::PreviousPowerOfTwo(1), 1);
    EXPECT_EQ(Util::Math::PreviousPowerOfTwo(2), 2);
    EXPECT_EQ(Util::Math::PreviousPowerOfTwo(3), 2);
    EXPECT_EQ(Util::Math::PreviousPowerOfTwo(5), 4);
    EXPECT_EQ(Util::Math::PreviousPowerOfTwo(16), 16);
    EXPECT_EQ(Util::Math::PreviousPowerOfTwo(17), 16);
    EXPECT_EQ(Util::Math::PreviousPowerOfTwo(1023), 512);
    EXPECT_EQ(Util::Math::PreviousPowerOfTwo(1024), 1024);
    EXPECT_EQ(Util::Math::PreviousPowerOfTwo(1025), 1024);
    EXPECT_EQ(Util::Math::PreviousPowerOfTwo(65535), 32768);
    EXPECT_EQ(Util::Math::PreviousPowerOfTwo(65536), 65536);
    EXPECT_EQ(Util::Math::PreviousPowerOfTwo(65537), 65536);
    EXPECT_EQ(Util::Math::PreviousPowerOfTwo(1000000), 524288);
}
} // namespace Moon::Test
