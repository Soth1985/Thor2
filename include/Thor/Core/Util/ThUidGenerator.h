#pragma once

#include <Thor/Core/Common.h>
#include <atomic>

namespace Thor
{
    class ThUidGenerator
    {
    public:
        ThUidGenerator();
        ThU64 GenerateSnowflake();
        ThU64 GenerateRandom();
        void SetConstant(ThU8 constant);
    private:
        ThU64 GetMilliseconds();
        static const ThU64 m_Epoch;
        static const ThU64 m_ConstantBits;
        static const ThU64 m_SequenceBits;
        static const ThU64 m_SequenceMask;
        static const ThU64 m_ConstantShift;
        static const ThU64 m_TimestampShift;
        ThU64 m_Constant;
        std::atomic<ThU64> m_Sequence;
        std::atomic<ThU64> m_LastTimestamp;
    };
}
