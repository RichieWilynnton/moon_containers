#include <AllocatorLib/managedSharedMemorySegmentAllocator.hpp>
#include <VectorLib/vector.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>

#include <iostream>

int main()
{
    auto segment = std::make_shared<boost::interprocess::managed_shared_memory>(
        boost::interprocess::open_only, "TestSegment");
    auto allocator = Moon::ManagedSharedMemorySegmentAllocator<int>(segment, "MyIntVector");

    Moon::Vector<int, Moon::ManagedSharedMemorySegmentAllocator<int>> vec(allocator);
    vec.Reserve(10); 
    for (const auto& elem : vec)
    {
         std::cout << "Consumed: " << elem << " ";
    }
    std::cout << std::endl;
    return 0;
}
