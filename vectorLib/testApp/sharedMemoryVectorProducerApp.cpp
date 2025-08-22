#include <AllocatorLib/managedSharedMemorySegmentAllocator.hpp>
#include <VectorLib/vector.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <iostream>

int main()
{
    auto segment = std::make_shared<boost::interprocess::managed_shared_memory>(
        boost::interprocess::open_or_create, "TestSegment", 65536);
    auto allocator = Moon::ManagedSharedMemorySegmentAllocator<int>(
        segment, "MyIntVector");
    Moon::Vector<int, Moon::ManagedSharedMemorySegmentAllocator<int>> vec(
        allocator);

    size_t* vec_size =
        segment->find_or_construct<size_t>("MyIntVectorSize")(0);

    vec.PushBack(42);
    vec.PushBack(42);
    vec.PushBack(100);

    for (const auto& elem : vec)
    {
        std::cout << "Pushed: " << elem << " ";
    }

    std::cout << std::endl;

    boost::interprocess::shared_memory_object::remove("TestSegment");
    return 0;
}
