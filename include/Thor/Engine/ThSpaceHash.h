#pragma once

#include <Thor/Engine/EngineForward.h>

namespace Thor
{
    class ThSpaceHash
    {
    public:
        ThSpaceHash()
            :
        m_Hash(UINT64_MAX)
        {
            
        }
        
        ThSpaceHash(ThU32 x, bool active)
            :
        m_Hash(0)
        {
            SetX(x);
            SetActive(active);
        }
        
        ThSpaceHash(ThU32 x, ThU32 y, bool active)
            :
        m_Hash(0)
        {
            SetX(x);
            SetY(y);
            SetActive(active);
        }
        
        ThSpaceHash(ThU32 x, ThU32 y, ThU32 z, bool active)
            :
        m_Hash(0)
        {
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
        
        void SetX(ThU32 x)
        {
            m_Hash &= ~m_XMask;
            m_Hash |= (ThU64(x) << m_XOffset);
        }
        
        void SetY(ThU32 y)
        {
            m_Hash &= ~m_YMask;
            m_Hash |= (ThU64(y) << m_YOffset);
        }
        
        void SetZ(ThU32 z)
        {
            m_Hash &= ~m_ZMask;
            m_Hash |= (ThU64(z));
        }
    private:
        static const ThI32 m_ActiveOffset = 63;
        static const ThI32 m_YOffset = 21;
        static const ThI32 m_XOffset = m_YOffset + 21;
        static const ThU64 m_ZMask = ThU64(0x1FFFFF);
        static const ThU64 m_YMask = ThU64(0x3FFFFE00000);
        static const ThU64 m_XMask = ThU64(0x7FFFFC0000000000);
        ThU64 m_Hash;
    };
}