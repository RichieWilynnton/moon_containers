#pragma once

#include <cstddef>
namespace Moon
{

template <typename T>
class Stack
{
   public:
    Stack();
    ~Stack();
    Stack(Stack& other);
    Stack(Stack&& other);
    Stack& operator=(Stack& other);
    Stack& operator=(Stack&& other);

    void Push();
    void Pop();
    size_t Size();
    bool Empty();

   private:
    size_t mCapacity;
    size_t mElems;
    T* mPtr;
};

}  // namespace Moon
