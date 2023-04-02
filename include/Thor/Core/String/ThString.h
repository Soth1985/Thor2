#pragma once

#include <Thor/Core/Common.h>

namespace Thor
{

class ThString
{
public:
    ThString();
    ThString(const ThChar* data);
    ThString(const ThChar* data, ThI32 hash, ThU32 length, ThU64 uid);
    ThString(const ThString& copy) = default;

    ThI32 GetHash()const
    {
        return m_Hash;
    }

    ThU64 GetLength()const
    {
        return m_Length / sizeof(ThChar);
    }

    ThU64 GetUid()const
    {
        return m_Uid;
    }

    const ThChar* GetData()const
    {
        return m_Data;
    }

    bool operator==(const ThString& rhs)const
    {
        return m_Hash == rhs.m_Hash && m_Length == rhs.m_Length;
    }
private:
    ThI32 m_Hash = 0;
    ThU64 m_Length = 0;
    ThU64 m_Uid = 0;
    const ThChar* m_Data = nullptr;
};

}
