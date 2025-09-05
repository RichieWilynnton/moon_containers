#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <CommonLib/math.hpp>
#include <MemoryLib/arena.hpp>

namespace Moon::Test
{

class ArenaMemoryBlockFixture : public ::testing::Test
{
   protected:
    void SetUp() override
    {
        ::testing::GTEST_FLAG(throw_on_failure) = true;
        ::testing::InitGoogleMock();
    }

    void TearDown() override {}

    ArenaChunkHeader* GetChunkHeaders(ArenaMemoryBlock& block)
    {
        return block.mChunkHeaders;
    }
};

TEST_F(ArenaMemoryBlockFixture,
       WHEN_new_chunk_is_created_THEN_header_should_be_allocated_beside_chunk)
{
    Moon::ArenaMemoryBlock block(4096);
    auto size = 512;
    auto chunk = block.CreateNewChunk(size);
    auto blockChunkHeader = GetChunkHeaders(block);

    EXPECT_NE(nullptr, blockChunkHeader);
    EXPECT_EQ(chunk, blockChunkHeader);

    auto totalAllocationSize = ArenaMemoryBlock::CalcTotalAllocationSize(size);
    auto allocation = chunk->GetData();
    EXPECT_EQ(chunk, reinterpret_cast<ArenaChunk*>(
                         reinterpret_cast<std::byte*>(allocation) + totalAllocationSize -
                         sizeof(ArenaChunkHeader)));
}

TEST_F(ArenaMemoryBlockFixture,
       WHEN_empty_chunks_are_created_THEN_linked_list_is_properly_formed)
{
    Moon::ArenaMemoryBlock block(8192);
    auto chunk1 = block.CreateNewChunk(512);
    auto chunk2 = block.CreateNewChunk(2048);
    auto chunk3 = block.CreateNewChunk(4096);
    auto chunk4 = block.CreateNewChunk(1024);

    auto curHeader = GetChunkHeaders(block);
    auto nodes = 0;
    do {
        nodes++;
        curHeader = curHeader->mNext;
    } while (curHeader != GetChunkHeaders(block));
    EXPECT_EQ(4, nodes);
}

TEST_F(
    ArenaMemoryBlockFixture,
    WHEN_empty_chunk_is_requested_AND_no_chunks_exists_THEN_nullptr_is_returned)
{
    Moon::ArenaMemoryBlock block(4096);
    auto chunk = block.RequestEmptyChunk(512);
    EXPECT_EQ(nullptr, chunk);
}

TEST_F(
    ArenaMemoryBlockFixture,
    WHEN_empty_chunk_is_requested_AND_an_empty_chunk_exists_THEN_chunk_is_returned)
{
    Moon::ArenaMemoryBlock block(4096);
    block.CreateNewChunk(2048);
    block.RequestEmptyChunk(2048);
    auto header = GetChunkHeaders(block);
    EXPECT_TRUE(header->mIsUsed);
}

TEST_F(
    ArenaMemoryBlockFixture,
    WHEN_empty_chunk_is_requested_AND_no_empty_chunks_exists_THEN_nullptr_is_returned)
{
    Moon::ArenaMemoryBlock block(4096);
    block.CreateNewChunk(2048);
    block.RequestEmptyChunk(2048);
    auto header = GetChunkHeaders(block);
    EXPECT_TRUE(header->mIsUsed);

    EXPECT_EQ(nullptr, block.RequestEmptyChunk(2048));
}

TEST_F(
    ArenaMemoryBlockFixture,
    WHEN_empty_chunk_is_requested_AND_a_chunk_was_freed_THEN_freed_chunk_is_reused)
{
    Moon::ArenaMemoryBlock block(8192);
    auto chunk1 = block.CreateNewChunk(512);

    auto chunk = block.RequestEmptyChunk(512);
    auto header = GetChunkHeaders(block);
    EXPECT_TRUE(header->mIsUsed);

    header->mIsUsed = false;
    chunk = block.RequestEmptyChunk(512);
    EXPECT_TRUE(header->mIsUsed);
}

TEST_F(ArenaMemoryBlockFixture,
       WHEN_one_empty_chunk_is_requested_THEN_only_one_chunk_is_marked_as_used)
{
    Moon::ArenaMemoryBlock block(8192);
    auto chunk1 = block.CreateNewChunk(512);
    auto chunk2 = block.CreateNewChunk(2048);
    auto chunk3 = block.CreateNewChunk(4096);
    auto chunk4 = block.CreateNewChunk(1024);

    auto chunk = block.RequestEmptyChunk(513);
    auto curHeader = GetChunkHeaders(block);
    auto usedChunks = 0;
    do {
        if (curHeader->mIsUsed)
        {
            usedChunks++;
        }
        curHeader = curHeader->mNext;
    } while (curHeader != GetChunkHeaders(block));
    EXPECT_EQ(1, usedChunks);
}

TEST_F(ArenaMemoryBlockFixture,
       WHEN_empty_chunk_is_requested_THEN_best_fit_chunk_is_marked_as_true)
{
    Moon::ArenaMemoryBlock block(8192);
    auto chunk1 = block.CreateNewChunk(512);
    auto chunk2 = block.CreateNewChunk(2048);
    auto chunk3 = block.CreateNewChunk(4096);
    auto chunk4 = block.CreateNewChunk(1024);

    auto chunk = block.RequestEmptyChunk(513);
    auto curHeader = GetChunkHeaders(block);
    auto usedChunkIndex = 0;
    auto index = 4;
    do {
        if (curHeader->mIsUsed)
        {
            usedChunkIndex = index;
        }
        ++index;
        curHeader = curHeader->mNext;
    } while (curHeader != GetChunkHeaders(block));
    EXPECT_EQ(4, usedChunkIndex);
}

TEST_F(ArenaMemoryBlockFixture,
       WHEN_remaining_size_is_queried_THEN_correct_size_is_returned)
{
    Moon::ArenaMemoryBlock block(8192);
    const auto initialSize = block.GetRemainingSize();

    const size_t size1 = 512;
    block.CreateNewChunk(size1);
    const size_t totalSize1 = ArenaMemoryBlock::CalcTotalAllocationSize(size1);
    EXPECT_EQ(initialSize - totalSize1, block.GetRemainingSize());

    const size_t size2 = 2048;
    block.CreateNewChunk(size2);
    const size_t totalSize2 = ArenaMemoryBlock::CalcTotalAllocationSize(size2);
    EXPECT_EQ(initialSize - totalSize1 - totalSize2, block.GetRemainingSize());

    const size_t size3 = 1024;
    block.CreateNewChunk(size3);
    const size_t totalSize3 = ArenaMemoryBlock::CalcTotalAllocationSize(size3);
    EXPECT_EQ(initialSize - totalSize1 - totalSize2 - totalSize3,
              block.GetRemainingSize());
}

}  // namespace Moon::Test
