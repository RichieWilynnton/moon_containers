#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <CommonLib/math.hpp>
#include <MemoryLib/arena.hpp>

namespace Moon::Test
{

class ArenaFixture : public ::testing::Test
{
   protected:
    void SetUp() override
    {
        ::testing::GTEST_FLAG(throw_on_failure) = true;
        ::testing::InitGoogleMock();
    }

    void TearDown() override {}

    auto GetDefaultAllocationSize(const Arena& arena)
    {
        return arena.mDefaultAllocationSize;
    }

    auto& GetMemoryBlocks(const Arena& arena)
    {
        return arena.mMemoryBlocks;
    }

    auto GetChunkHeaders(const ArenaMemoryBlock& block)
    {
        return block.mChunkHeaders;
    }

    auto IsChunkUsed(const ArenaChunk* chunk)
    {
        return static_cast<const ArenaChunkHeader*>(chunk)->mIsUsed;
    }

    uint64_t mPageSize = 1024;
};

TEST_F(ArenaFixture,
       WHEN_arena_is_created_THEN_default_allocation_size_is_page_aligned)
{
    Arena arena(500);
    EXPECT_EQ(GetDefaultAllocationSize(arena),
              Util::Math::AlignSize(500, mPageSize));
}

TEST_F(
    ArenaFixture,
    WHEN_request_chunk_with_no_blocks_THEN_new_block_is_created_and_chunk_returned)
{
    Arena arena(1024);
    auto chunk = arena.RequestChunk(512);

    EXPECT_NE(chunk, nullptr);
    auto& blocks = GetMemoryBlocks(arena);
    EXPECT_EQ(blocks.size(), 1);
    EXPECT_EQ(GetChunkHeaders(blocks[0]), chunk);
}

TEST_F(ArenaFixture,
       WHEN_request_chunk_AND_block_has_sufficient_space_THEN_existing_block_is_used)
{
    Arena arena(2048);
    ArenaChunk* chunk1 = arena.RequestChunk(512);
    ArenaChunk* chunk2 = arena.RequestChunk(512);

    auto chunkHeader1 = static_cast<ArenaChunkHeader*>(chunk1);
    auto chunkHeader2 = static_cast<ArenaChunkHeader*>(chunk2);

    EXPECT_NE(chunk1, nullptr);
    EXPECT_NE(chunk2, nullptr);
    EXPECT_NE(chunk1, chunk2);    
    EXPECT_EQ(GetMemoryBlocks(arena).size(), 1); 
}

TEST_F(ArenaFixture,
       WHEN_request_chunk_with_insufficient_space_THEN_new_block_is_created)
{
    Arena arena(1024);
    ArenaChunk* chunk1 = arena.RequestChunk(800);
    ArenaChunk* chunk2 = arena.RequestChunk(800);

    EXPECT_NE(chunk1, nullptr);
    EXPECT_NE(chunk2, nullptr);
    auto& memoryBlocks = GetMemoryBlocks(arena);
    EXPECT_EQ(memoryBlocks.size(), 2);
    EXPECT_NE(chunk1, chunk2);
    EXPECT_EQ(chunk1, GetChunkHeaders(memoryBlocks[0]));
    EXPECT_EQ(chunk2, GetChunkHeaders(memoryBlocks[1]));
}

TEST_F(ArenaFixture,
       WHEN_request_chunk_with_size_greater_than_default_allocation_size_THEN_new_block_is_created_with_greater_than_given_size)
{
    const auto defaultSize = 1024;
    Arena arena(defaultSize);
    const auto& memoryBlocks = GetMemoryBlocks(arena);
    // size guaranteed to be bigger than default size
    ArenaChunk* chunk = arena.RequestChunk((defaultSize + sizeof(ArenaChunkHeader)) * 3);
    EXPECT_NE(chunk, nullptr);
    EXPECT_EQ(memoryBlocks.size(), 1); 
    EXPECT_GE(memoryBlocks[0].GetCapacity(), defaultSize);
}

TEST_F(ArenaFixture, WHEN_release_chunk_is_called_THEN_chunk_is_marked_unused)
{
    Arena arena(1024);
    ArenaChunk* chunk = arena.RequestChunk(512);

    EXPECT_TRUE(IsChunkUsed(chunk));
    arena.ReleaseChunk(chunk);
    EXPECT_FALSE(IsChunkUsed(chunk));
}

TEST_F(ArenaFixture, WHEN_release_nullptr_chunk_THEN_no_crash_occurs)
{
    Arena arena(1024);
    arena.ReleaseChunk(nullptr);
    EXPECT_TRUE(true);
}

TEST_F(ArenaFixture, WHEN_request_empty_chunk_THEN_reuses_unused_chunk)
{
    Arena arena(2048);
    ArenaChunk* chunk1 = arena.RequestChunk(512);
    arena.ReleaseChunk(chunk1);
    EXPECT_FALSE(IsChunkUsed(chunk1));

    ArenaChunk* chunk2 = arena.RequestChunk(512);

    EXPECT_EQ(chunk1, chunk2);  
    EXPECT_TRUE(IsChunkUsed(chunk2));
}

TEST_F(ArenaFixture, WHEN_memory_block_is_destroyed_THEN_memory_is_freed)
{
    Arena arena(2048);
    {
        ArenaChunk* chunk = arena.RequestChunk(512);
    }
    // Test passes if no memory leaks occur (cannot directly test free, but no
    // crashes expected)
    EXPECT_TRUE(true);
}

}  // namespace Moon::Test
