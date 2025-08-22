#pragma once

#include <cstddef>
namespace Moon
{

template <typename T>
class OpenAddressingCollisionHandlerIterator
{
   public:
    OpenAddressingCollisionHandlerIterator& operator++() noexcept;
    OpenAddressingCollisionHandlerIterator operator++(int) noexcept;
    OpenAddressingCollisionHandlerIterator& operator--() noexcept;
    OpenAddressingCollisionHandlerIterator operator--(int) noexcept;
    OpenAddressingCollisionHandlerIterator operator+(int offset) const noexcept;
    OpenAddressingCollisionHandlerIterator operator-(int offset) const noexcept;
    size_t operator-(
        const OpenAddressingCollisionHandlerIterator& other) const noexcept;

    bool operator==(
        const OpenAddressingCollisionHandlerIterator& other) const noexcept;
    bool operator!=(
        const OpenAddressingCollisionHandlerIterator& other) const noexcept;
    bool operator>(
        const OpenAddressingCollisionHandlerIterator& other) const noexcept;
    bool operator<(
        const OpenAddressingCollisionHandlerIterator& other) const noexcept;
    bool operator>=(
        const OpenAddressingCollisionHandlerIterator& other) const noexcept;
    bool operator<=(
        const OpenAddressingCollisionHandlerIterator& other) const noexcept;

    T& operator*();
    T* operator->() noexcept;

   private:
    OpenAddressingCollisionHandlerIterator();

   private:
    T* mPtr;
};

}  // namespace Moon
