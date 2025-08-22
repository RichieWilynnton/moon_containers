#pragma once

#include <MapLib/hashmap.hpp>

namespace Moon
{

template <typename Key, typename Value, typename Hasher,
          template <typename> typename CollisionHandler>
void HashMap<Key, Value, Hasher, CollisionHandler>::Insert(const Key& key,
                                                           const Value& value)
{
    const auto hash = Hasher::Hash(key);
    mCollisionHandler.Insert(hash, value);
}

template <typename Key, typename Value, typename Hasher,
          template <typename> typename CollisionHandler>
void HashMap<Key, Value, Hasher, CollisionHandler>::Delete(const Key& key)
{
    const auto hash = Hasher::Hash(key);
    mCollisionHandler.Delete(hash);
}

template <typename Key, typename Value, typename Hasher,
          template <typename> typename CollisionHandler>
void HashMap<Key, Value, Hasher, CollisionHandler>::Clear()
{
    mCollisionHandler.Clear();
}

template <typename Key, typename Value, typename Hasher,
          template <typename> typename CollisionHandler>
Value& HashMap<Key, Value, Hasher, CollisionHandler>::operator[](const Key& key)
{
    const auto& keyValuePair = mCollisionHandler.LookupOrDefaultConstruct();
    return keyValuePair.mValue;
}

template <typename Key, typename Value, typename Hasher,
          template <typename> typename CollisionHandler>
HashMap<Key, Value, Hasher, CollisionHandler>::IteratorType HashMap<Key, Value, Hasher, CollisionHandler>::Find(const Key& key)
{
    const auto hash = Hasher::Hash(key);
    return mCollisionHandler.Find(hash); 
}

}  // namespace Moon
