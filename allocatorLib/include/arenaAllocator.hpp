#pragma once

#include <MemoryLib/arena.hpp>
#include <cstddef>
#include <cstdlib>

namespace Moon
{
template <typename T>
class ArenaAllocator
{
   public:
    ArenaAllocator(Arena& arena) : mArena(arena) {}

    T* Allocate(size_t size);

    void Deallocate(T*& ptr);

    template <typename... Args>
    void Construct(T* ptr, Args&&... args);

    void Destruct(T* ptr) noexcept;

    size_t GetNewCapacity(const size_t numOfElems) noexcept;

    size_t GetStartingCapacity() const noexcept;

   private:
    Arena& mArena;
    ArenaChunk* mCurrentChunk = nullptr;
};
}  // namespace Moon

#include <AllocatorLib/arenaAllocator.ipp>
