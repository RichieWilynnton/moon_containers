#pragma once

#include <cstddef>
#include <cstdint>

namespace Moon::Util
{

class Math
{
   public:
    static size_t NextPowerOfTwo(size_t n)
    {
        if (n == 0)
            return 1;
        --n;
        for (size_t i = 1; i < sizeof(size_t) * 8; i <<= 1)
        {
            n |= n >> i;
        }
        return n + 1;
    }

    static size_t PreviousPowerOfTwo(size_t n)
    {
        if (n == 0)
            return 0;
        size_t power = 1;
        while (power <= n)
        {
            power <<= 1;
        }
        return power >> 1;
    }

    template <typename T>
    static T* AlignPtr(T* ptr, const size_t alignment)
    {
        const auto ptrInt = reinterpret_cast<uintptr_t>(ptr);
        const auto offset = ptrInt % alignment;
        if (offset == 0)
            return reinterpret_cast<T*>(ptr);
        const auto adjustment = alignment - offset;
        return reinterpret_cast<T*>(ptrInt + adjustment);
    }

    static size_t AlignSize(const size_t size, const size_t alignment)
    {
        const auto offset = size % alignment;
        if (offset == 0)
            return size;
        const auto padding = alignment - offset;
        return size + padding;
    }
};
}  // namespace Moon::Util
