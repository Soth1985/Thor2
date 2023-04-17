#pragma once

#include <Thor/Core/Common.h>

namespace Thor
{

template <int Size>
constexpr ThU64 PolynomialHash(const ThChar (&string)[Size]) 
{
    const ThI32 p = 31;
    const ThI32 m = 1e9 + 9;
    ThU64 hashValue = 0;
    ThU64 Pow = 1;

    for (int i = 0; i < Size; ++i) 
    {
        hashValue = (hashValue + (string[i] - 'a' + 1) * Pow) % m;
        Pow = (Pow * p) % m;
    }

    return hashValue;
}

}
