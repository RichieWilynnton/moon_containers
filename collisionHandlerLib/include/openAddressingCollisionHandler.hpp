#pragma once

#include <AllocatorLib/heapAllocator.hpp>
#include <CollisionHandlerLib/openAddressingCollisionHandlerIterator.hpp>
#include <memory>  // todo: change
#include <vector>  // todo: change

namespace Moon
{

template <typename Data,
          typename Allocator = HeapAllocator<Data>>  // todo: allocator unused
class OpenAddressingCollisionHandler
{
   private:
    struct Node
    {
        size_t mHash;
        std::unique_ptr<Data> mData;
    };

   public:
    using IteratorType = OpenAddressingCollisionHandlerIterator<Data>;

    OpenAddressingCollisionHandler();
    OpenAddressingCollisionHandler(const size_t bucketSize);

    void Insert(const size_t hash, const Data& data);
    void Delete(const size_t hash);
    Data& LookupOrDefaultConstruct(const size_t hash);
    IteratorType Find(const size_t hash);
    IteratorType begin() const;
    IteratorType end() const;
    IteratorType Begin() const;
    IteratorType End() const;

   private:
    void ResizeAndRehash(const size_t newSize);

   private:
    size_t mLoadFactor;
    std::vector<Node> mHashTable;
    size_t mElemCount{0};
    size_t mTombstoneCount{0};
    // note: tombstones are deleted elements that need to remain so
    // that the probing sequence can continue correctly. mHash == mHashTable.size() -> tombstone
};
}  // namespace Moon

#include <CollisionHandlerLib/openAddressingCollisionHandler.ipp>
