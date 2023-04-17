#include <Thor/Core/Util/ThUidGenerator.h>
#include <Thor/Core/Debug/ThLogger.h>
#include <chrono>
#include <random>
#include <cstdlib>
#include <thread>

using namespace Thor;

static constexpr ThU64 m_Epoch = 1474173115946;
static constexpr ThU64 m_ConstantBits = 8;
static constexpr ThU64 m_SequenceBits = 14;
static constexpr ThU64 m_SequenceMask = 0x7FFF;
static constexpr ThU64 m_ConstantShift = m_SequenceBits;
static constexpr ThU64 m_TimestampShift = m_ConstantBits + m_SequenceBits;

ThUidGenerator::ThUidGenerator()
    :
m_Constant(0),
m_LastTimestamp(0),
m_Sequence(0)
{
    
}

ThU64 ThUidGenerator::GenerateSnowflake(ThU8 constant)
{
    ThU64 timestamp = GetMilliseconds();
    ThU64 sequence = m_Sequence++;
    
    if (timestamp < m_LastTimestamp)
    {
        THOR_ERR("System time is reset or is going backwards", ThLogger::TagSystem);
        return UINT64_MAX;
    }
    
    if (m_LastTimestamp == timestamp)
    {
        sequence = sequence & m_SequenceMask;
        if (sequence == 0)
        {
            THOR_WRN("Generating too many uids", ThLogger::TagSystem);
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
            return GenerateSnowflake(constant);
        }
    }
    else
    {
        sequence = 0;
        m_Sequence = 0;
    }
    
    if (timestamp > m_LastTimestamp)
        m_LastTimestamp = timestamp;
    
    ThU64 result = ((timestamp - m_Epoch) << m_TimestampShift) | (m_Constant << m_ConstantShift) | sequence;
    return result;
}

ThU64 ThUidGenerator::GenerateRandom()
{
    using namespace std::chrono;
    static thread_local std::mt19937_64 generator(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
    return generator();
}

ThU64 ThUidGenerator::GetMilliseconds()
{
    using namespace std::chrono;
    milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    return ms.count();
}
