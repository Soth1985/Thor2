#pragma once
#include <Thor/Core/Common.h>

namespace Thor
{
    namespace Hash
    {
        ThU32 Murmur3(const ThU8* key, ThSize len, ThU32 seed);
    }
}
