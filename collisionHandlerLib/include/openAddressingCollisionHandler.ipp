#pragma once

#include <CollisionHandlerLib/openAddressingCollisionHandler.hpp>

namespace Moon
{

template <typename Data, typename Allocator>
void OpenAddressingCollisionHandler<Data, Allocator>::Insert(const size_t hash,
                                                             const Data& data)
{
    if (static_cast<double>(mElemCount) / mHashTable.size() > mLoadFactor)
    {
        ResizeAndRehash(mHashTable.size() * 2);
    }
    const size_t index = hash % mHashTable.size();
    size_t currentIndex = index;
    while (mHashTable[currentIndex].mData &&
           mHashTable[currentIndex].mData->mKey != data.mKey)
    {
        // todo: linear probing for now, make this configurable
        currentIndex = (currentIndex + 1) % mHashTable.size();
    }

    if (mHashTable[currentIndex])
    {
        mHashTable[currentIndex].mData->mValue = data.mValue;
    }
    else
    {
        mHashTable[currentIndex].mData = std::make_unique<Data>(data);
        ++mElemCount;
    }
}

template <typename Data, typename Allocator>
void OpenAddressingCollisionHandler<Data, Allocator>::Delete(const size_t hash)
{
    size_t currentIndex = hash % mHashTable.size();
    while (mHashTable[currentIndex]->mData &&
           mHashTable[currentIndex]->mHash == hash)
    {
        currentIndex = (currentIndex + 1) % mHashTable.size();
    }

    if (mHashTable[currentIndex].mData &&
        mHashTable[currentIndex].mData->mHash == hash)
    {
        mHashTable[currentIndex].mData.reset(nullptr);
        --mElemCount;
        ++mTombstoneCount;
    }
}

template <typename Data, typename Allocator>
Data& OpenAddressingCollisionHandler<Data, Allocator>::LookupOrDefaultConstruct(
    const size_t hash)
{
    size_t currentIndex = hash % mHashTable.size();
    while (mHashTable[currentIndex].mData &&
           mHashTable[currentIndex].mData->mHash != hash)
    {
        currentIndex = (currentIndex + 1) % mHashTable.size();
    }

    if (!mHashTable[currentIndex].mData)
    {
        mHashTable[currentIndex].mData =
            std::make_unique<Data>(Data{hash, Data::DefaultValue()});
        ++mElemCount;
    }

    return *mHashTable[currentIndex].mData;
}

template <typename Data, typename Allocator>
typename OpenAddressingCollisionHandler<Data, Allocator>::IteratorType
OpenAddressingCollisionHandler<Data, Allocator>::Find(const size_t hash)
{
    size_t currentIndex = hash % mHashTable.size();
    while (mHashTable[currentIndex].mData &&
           mHashTable[currentIndex].mData->mHash != hash)
    {
        currentIndex = (currentIndex + 1) % mHashTable.size();
    }

    if (mHashTable[currentIndex].mData &&
        mHashTable[currentIndex].mData->mHash == hash)
    {
        return IteratorType(&mHashTable[currentIndex].mData);
    }
    return IteratorType(nullptr); 
}

template <typename Data, typename Allocator>
void OpenAddressingCollisionHandler<Data, Allocator>::ResizeAndRehash(
    const size_t newSize)
{
    std::vector<Node> newHashTable(newSize);
    for (const auto& node : mHashTable)
    {
        if (node.mData)
        {
            size_t currentIndex = node.mHash % newSize;
            while (newHashTable[currentIndex].mData)
            {
                currentIndex = (currentIndex + 1) % newSize;
            }
            newHashTable[currentIndex] = node;
        }
    }
    mHashTable = std::move(newHashTable);
    mTombstoneCount = 0;
}

template <typename Data, typename Allocator>
typename OpenAddressingCollisionHandler<Data, Allocator>::IteratorType OpenAddressingCollisionHandler<Data, Allocator>::begin() const
{
    for (size_t i = 0; i < mHashTable.size(); ++i)
    {
        if (mHashTable[i].mData)
        {
            return IteratorType(&mHashTable[i].mData);
        }
    }
}

template <typename Data, typename Allocator>
typename OpenAddressingCollisionHandler<Data, Allocator>::IteratorType OpenAddressingCollisionHandler<Data, Allocator>::Begin() const
{
    return begin();
}

template <typename Data, typename Allocator>
typename OpenAddressingCollisionHandler<Data, Allocator>::IteratorType OpenAddressingCollisionHandler<Data, Allocator>::end() const
{
    return IteratorType(nullptr); 
}

template <typename Data, typename Allocator>
typename OpenAddressingCollisionHandler<Data, Allocator>::IteratorType OpenAddressingCollisionHandler<Data, Allocator>::End() const
{
    return end();
}

}  // namespace Moon
