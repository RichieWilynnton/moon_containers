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

    static T* Allocate(size_t size);

    static void Deallocate(T*& ptr);

    template <typename... Args>
    static void Construct(T* ptr, Args&&... args);

    static void Destruct(T* ptr) noexcept;

    static size_t GetNewCapacity(const size_t numOfElems) noexcept;

    size_t GetStartingCapacity() const noexcept
    {
        return mArena.GetCapacity();
    }

   private:
    Arena& mArena;
};
}  // namespace Moon

#include <AllocatorLib/arenaAllocator.ipp>
