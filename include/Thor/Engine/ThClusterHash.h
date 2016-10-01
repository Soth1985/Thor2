#pragma once

#include <Thor/Engine/EngineForward.h>
#include <Thor/Core/Containers/ThHash.h>

namespace Thor
{
    class ThClusterHash
    {
    public:
        ThClusterHash()
            :
        m_Hash(UINT64_MAX)
        {
            
        }
        
        ThClusterHash(ThU32 layer, ThU32 x)
            :
        m_Hash(0)
        {
            SetLayer(layer);
            SetX(x);
        }
        
        ThClusterHash(ThU32 layer, ThU32 x, ThU32 y)
            :
        m_Hash(0)
        {
            SetLayer(layer);
            SetX(x);
            SetY(y);
        }
        
        ThClusterHash(ThU32 layer, ThU32 x, ThU32 y, ThU32 z)
            :
        m_Hash(0)
        {
            SetLayer(layer);
            SetX(x);
            SetY(y);
            SetZ(z);
        }
        
        operator bool()const
        {
            return m_Hash != UINT64_MAX;
        }
        
        bool operator==(const ThClusterHash& rhs)const
        {
            return m_Hash == rhs.m_Hash;
        }
        
        ThU32 GetX()const
        {
            return (m_Hash & m_XMask) >> m_XOffset;
        }
        
        ThU32 GetY()const
        {
            return (m_Hash & m_YMask) >> m_YOffset;
        }
        
        ThU32 GetZ()const
        {
            return (m_Hash & m_ZMask);
        }
        
        ThU32 GetLayer()const
        {
            return (m_Hash & m_LayerMask) >> m_LayerOffset;
        }
        
        void SetX(ThU32 x)
        {
            m_Hash &= ~m_XMask;
            m_Hash |= (ThU64(x) << m_XOffset) & m_XMask;
        }
        
        void SetY(ThU32 y)
        {
            m_Hash &= ~m_YMask;
            m_Hash |= (ThU64(y) << m_YOffset) & m_YMask;
        }
        
        void SetZ(ThU32 z)
        {
            m_Hash &= ~m_ZMask;
            m_Hash |= (ThU64(z)) & m_ZMask;
        }
        
        void SetLayer(ThU32 layer)
        {
            m_Hash &= ~m_LayerMask;
            m_Hash |= (ThU64(layer) << m_LayerOffset) & m_LayerMask;
        }
        
        ThU64 GetHash()const
        {
            return m_Hash;
        }
        
        ThU32 GetMaxX()const
        {
            return m_XMask >> m_XOffset;
        }
        
        ThU32 GetMaxY()const
        {
            return m_YMask >> m_YOffset;
        }
        
        ThU32 GetMaxZ()const
        {
            return m_ZMask;
        }
    private:
        static const ThI32 m_XBits = 18;
        static const ThI32 m_YBits = 18;
        static const ThI32 m_ZBits = 18;
        static const ThI32 m_LayerBits = 10;
        static const ThU64 m_AllBits = ThU64(0xFFFFFFFFFFFFFFFF);
        static const ThI32 m_YOffset = m_ZBits;
        static const ThI32 m_XOffset = m_YOffset + m_YBits;
        static const ThI32 m_LayerOffset = m_XOffset + m_XBits;
        static const ThU64 m_LayerMask = ThU64(m_AllBits >> (64 - m_LayerBits)) << m_LayerOffset;
        static const ThU64 m_XMask = ThU64(m_AllBits >> (64 - m_XBits)) << m_XOffset;
        static const ThU64 m_YMask = ThU64(m_AllBits >> (64 - m_YBits)) << m_YOffset;
        static const ThU64 m_ZMask = ThU64(m_AllBits >> (64 - m_ZBits));
        
        ThU64 m_Hash;
    };
    
    template <>
    struct ThHash<ThClusterHash>
    {
        static inline ThSize HashCode(const ThClusterHash& cluster)
        {
            return cluster.GetHash();
        }
    };
}
