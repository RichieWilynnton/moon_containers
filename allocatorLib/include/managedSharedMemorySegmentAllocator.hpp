#pragma once

#include <boost/interprocess/managed_shared_memory.hpp>
#include <cstddef>
#include <cstdlib>

namespace Moon
{
// Note: The allocator does not own the memory segment, it is expected to be
// created and managed externally. The allocator will allocate memory within
// segment.
template <typename T>
class ManagedSharedMemorySegmentAllocator
{
    using ManagedSharedMemory = boost::interprocess::managed_shared_memory;

   public:
    static constexpr size_t STARTING_CAPACITY = 1;

   public:
    ManagedSharedMemorySegmentAllocator(
        std::shared_ptr<ManagedSharedMemory> segment,
        const std::string& regionName)
        : mManagedSegment(segment), mRegionName(regionName), mCapacity(0)
    {
    }

    ManagedSharedMemorySegmentAllocator(
        const ManagedSharedMemorySegmentAllocator&) = default;

    T* Allocate(size_t size);

    void Deallocate(T*& ptr);

    template <typename... Args>
    void Construct(T* ptr, Args&&... args);

    void Destruct(T* ptr) noexcept;

    size_t GetNewCapacity(const size_t numOfElems) noexcept;

    void Destroy();

   private:
        
   private:
    std::shared_ptr<ManagedSharedMemory> mManagedSegment;
    std::string mRegionName;
    size_t mCapacity;
};
}  // namespace Moon

#include <AllocatorLib/managedSharedMemorySegmentAllocator.ipp>
