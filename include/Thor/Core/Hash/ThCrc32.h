#pragma once
#include <Thor/Core/Common.h>

namespace Thor
{
    namespace Hash
    {
        ThU32 Crc32(const ThU8* msg, ThSize size, ThU32 crcinit);
    }
}
