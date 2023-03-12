#include <Thor/Core/String/ThString.h>
#include <Thor/Core/ThCore.h>
#include <Thor/Core/Util/ThUidGenerator.h>
#include <Thor/Core/Hash/ThMurmurHash.h>

using namespace Thor;

ThString::ThString()
{

}

ThString::ThString(const ThChar* data)
    : 
m_Data(data)
{
    m_Length = strlen(data);
    m_Uid = ThCore::GetUidGenerator()->GenerateSnowflake();
    m_Hash = Hash::Murmur3((ThU8*)data, m_Length, m_Uid);
}

ThString::ThString(const ThChar* data, ThI32 hash, ThU32 length, ThU64 uid)
    :
m_Data(data),
m_Hash(hash),
m_Length(length),
m_Uid(uid)
{

}