#pragma once

#include <cstddef>
#include <cstdint>

namespace Moon
{

class ArenaChunk
{
   public:
    ArenaChunk(std::byte* start, const size_t capacity) : mStart(start), mCapacity(capacity)
    {}
    void Reset();
    void* GetData();
    size_t GetCapacity();

   private:
    std::byte* mStart;
    std::uint64_t mCapacity;

};
}  // namespace Moon
