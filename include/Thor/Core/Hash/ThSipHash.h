#pragma once
#include <Thor/Core/Common.h>

namespace Thor
{
    namespace Hash
    {
        ThI32 SipHash(const ThU8* in, ThSize inlen, const ThU8* k, ThU8* out, ThSize outlen);
    }
}
