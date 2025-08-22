#pragma once

#include <MapLib/hashMap.hpp>
#include <functional>

namespace Moon
{

template <typename Key, typename Value, typename Hasher = std::hash<Key>,
          template <typename> typename CollisionHandler = OpenAddressingCollisionHandler<KeyValuePair>>
class HashMap
{
   public:
    struct KeyValuePair
    {
        Key mKey;
        Value mValue;
    };
    using IteratorType = typename CollisionHandler<KeyValuePair>::IteratorType;
    HashMap() : mCollisionHandler(CollisionHandler<KeyValuePair>()) {}

    HashMap(const size_t bucketCount)
        : mCollisionHandler(CollisionHandler<KeyValuePair>(bucketCount))
    {
    }

    HashMap(const HashMap& other) : mCollisionHandler(other.mCollisionHandler)
    {
    }

    HashMap(HashMap&& other)
        : mCollisionHandler(std::move(other.mCollisionHandler))
    {
    }

    HashMap& operator=(const HashMap& other)
    {
        if (this != &other)
        {
            mCollisionHandler = other.mCollisionHandler;
        }
    }

    HashMap& operator=(HashMap&& other)
    {
        if (this != &other)
        {
            mCollisionHandler = std::move(other.mCollisionHandler);
        }
    }

    void Insert(const Key& key, const Value& value);
    void Delete(const Key& key);
    void Clear();
    IteratorType Find(const Key& key);

    Value& operator[](const Key& key);

   private:
    CollisionHandler<KeyValuePair> mCollisionHandler;
};

}  // namespace Moon

#include <MapLib/hashmap.ipp>
