#pragma once

#include <cstddef>
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
};
}  // namespace Moon::Util
