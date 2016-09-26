#pragma once

#include <Thor/Engine/EngineForward.h>

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
        
        ThClusterHash(ThU32 layer, ThU32 x, bool active)
            :
        m_Hash(0)
        {
            SetLayer(layer);
            SetX(x);
            SetActive(active);
        }
        
        ThClusterHash(ThU32 layer, ThU32 x, ThU32 y, bool active)
            :
        m_Hash(0)
        {
            SetLayer(layer);
            SetX(x);
            SetY(y);
            SetActive(active);
        }
        
        ThSpaceHash(ThU32 layer, ThU32 x, ThU32 y, ThU32 z, bool active)
            :
        m_Hash(0)
        {
            SetLayer(layer);
            SetX(x);
            SetY(y);
            SetZ(z);
            SetActive(active);
        }
        
        operator bool()const
        {
            return m_Hash != UINT64_MAX;
        }
        
        bool IsActive()const
        {
            return m_Hash >> m_ActiveOffset;
        }
        
        void SetActive(bool active)
        {
            if (active)
                m_Hash |= (ThU64(1) << m_ActiveOffset);
            else
                m_Hash &= ~(ThU64(1) << m_ActiveOffset);
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
    private:
        static const ThI32 m_XBits = 21;
        static const ThI32 m_YBits = 10;
        static const ThI32 m_ZBits = 21;
        static const ThI32 m_LayerBits = 11;
        static const ThI32 m_ActiveOffset = 63;
        static const ThI32 m_YOffset = m_ZBits;
        static const ThI32 m_XOffset = m_YOffset + m_YBits;
        static const ThI32 m_LayerOffset = m_XOffset + m_XBits;
        static const ThU64 m_LayerMask = ThU64(0x7FF0000000000000);
        static const ThU64 m_XMask = ThU64(0xFFFFF80000000);
        static const ThU64 m_YMask = ThU64(0x7FE00000);
        static const ThU64 m_ZMask = ThU64(0x1FFFFF);
        
        ThU64 m_Hash;
    };
}