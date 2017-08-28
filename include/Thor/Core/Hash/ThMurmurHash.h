#pragma once
#include <Thor/Core/Common.h>

namespace Thor
{
    namespace Hash
    {
        ThU32 Murmur3(const ThU8* key, ThSize len, ThU32 seed);
        ThU32 Murmur2(const ThU8* key, ThSize len, ThU32 seed);
        ThU64 Murmur2_64(const ThU8* key, ThSize len, ThU64 seed);
    }
}
