#pragma once

#include <Thor/Core/Common.h>
#include <atomic>

namespace Thor
{
    class ThUidGenerator
    {
    public:
        ThUidGenerator();
        ThU64 GenerateSnowflake(ThU8 constant);
        ThU64 GenerateRandom();
    private:
        ThU64 GetMilliseconds();
        ThU64 m_Constant;
        std::atomic<ThU64> m_Sequence;
        std::atomic<ThU64> m_LastTimestamp;
    };
}
