#include <Thor/Core/Util/ThUidGenerator.h>
#include <Thor/Core/Debug/ThLogger.h>
#include <chrono>
#include <random>
#include <cstdlib>
#include <thread>

using namespace Thor;

const ThU64 ThUidGenerator::m_Epoch = 1474173115946;
const ThU64 ThUidGenerator::m_ConstantBits = 8;
const ThU64 ThUidGenerator::m_SequenceBits = 14;
const ThU64 ThUidGenerator::m_SequenceMask = 0x7FFF;
const ThU64 ThUidGenerator::m_ConstantShift = m_SequenceBits;
const ThU64 ThUidGenerator::m_TimestampShift = m_ConstantBits + m_SequenceBits;

ThUidGenerator::ThUidGenerator()
    :
m_Constant(0),
m_LastTimestamp(-1),
m_Sequence(0)
{
    
}

ThU64 ThUidGenerator::GenerateSnowflake()
{
    ThU64 timestamp = GetMilliseconds();
    ThU64 sequence = m_Sequence++;
    
    if (timestamp < m_LastTimestamp)
    {
        THOR_ERR("System time is reset or is going backwards", coreSysLogTag);
        return UINT64_MAX;
    }
    
    if (m_LastTimestamp == timestamp)
    {
        sequence = sequence & m_SequenceMask;
        if (sequence == 0)
        {
            THOR_WRN("Generating too many uids", coreSysLogTag);
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
            return GenerateSnowflake();
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

void ThUidGenerator::SetSnowflakeConstant(ThU8 constant)
{
    m_Constant = constant;
}

ThU64 ThUidGenerator::GetMilliseconds()
{
    using namespace std::chrono;
    milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    return ms.count();
}
