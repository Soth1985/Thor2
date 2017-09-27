#pragma once

#include <Thor/Core/Common.h>
#include <Thor/Core/Containers/ThHash.h>

namespace Thor
{    
    class ThStringHash32
    {
    public:
        ThStringHash32()
        :
        m_Hash(UINT32_MAX)
        {}
        
        ThStringHash32(const char* str)
        {
            m_Hash = Thor::Hash::Murmur3(str, strlen(str), 0);
        }
        
        ThU32 GetHash()const
        {
            return m_Hash;
        }
        
        bool operator==(const ThStringHash& rhs)const
        {
            return m_Hash == rhs.m_Hash;
        }
        
        bool operator!=(const ThStringHash& rhs)const
        {
            return m_Hash != rhs.m_Hash;
        }
        
        operator bool()const
        {
            return m_Hash != UINT32_MAX;
        }
        
    private:
        ThU32 m_Hash;
    };
    
    template<>
    struct ThHash< ThStringHash32 >
    {
        static inline ThSize HashCode(const ThStringHash32& key)
        {
            return key.GetHash();
        }
    };
    
    class ThStringHash64
    {
    public:
        ThStringHash64()
        :
        m_Hash(UINT64_MAX)
        {}
        
        ThStringHash64(const char* str)
        {
            m_Hash = Thor::Hash::Murmur2_64(str, strlen(str), 0);
        }
        
        ThU32 GetHash()const
        {
            return m_Hash;
        }
        
        bool operator==(const ThStringHash& rhs)const
        {
            return m_Hash == rhs.m_Hash;
        }
        
        bool operator!=(const ThStringHash& rhs)const
        {
            return m_Hash != rhs.m_Hash;
        }
        
        operator bool()const
        {
            return m_Hash != UINT64_MAX;
        }
        
    private:
        ThU64 m_Hash;
    };
    
    template<>
    struct ThHash< ThStringHash64 >
    {
        static inline ThSize HashCode(const ThStringHash64& key)
        {
            return key.GetHash();
        }
    };
}

