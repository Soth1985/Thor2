#pragma once

#include <Thor/Engine/EngineForward.h>
#include <Thor/Core/Containers/ThHash.h>

namespace Thor
{
    class ThEntity
    {
    public:
        ThEntity()
            :
        m_Uid(0)
        {
            
        }
        
        explicit ThEntity(ThU64 uid)
            :
        m_Uid(uid)
        {
            
        }
        
        operator bool()const
        {
            return m_Uid != 0;
        }
        
        ThU64 Uid()const
        {
            return m_Uid;
        }
        
    private:
        ThU64 m_Uid;
    };
    
    namespace Private
    {
        inline ThSize HashFunc(const ThEntity& ent)
        {
            return ent.Uid();
        }
    }
}