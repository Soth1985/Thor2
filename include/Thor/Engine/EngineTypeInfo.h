#pragma once

#include <Thor/Core/Common.h>

namespace Thor
{
    template <class Class>
    class ThTypeID
    {
    public:
        explicit ThTypeID(ThU32 type, ThU32 kind)
            :
        m_Type(type),
        m_Kind(kind)
        {}
        
        bool operator==(const ThTypeID& other)const
        {
            return m_Type == other.m_Type && m_Kind == other.m_Kind;
        }
        
        ThU32 Type()const
        {
            return m_Type;
        }
        
        ThU32 Kind()const
        {
            return m_Kind;
        }
        
    private:
        ThU32 m_Type : 22;
        ThU32 m_Kind : 10;
    };
    
    template <class Class, ThU32 TypeID, ThU32 KindID>
    class ThKey
    {
    public:
        ThKey()
            :
        m_Index(-1)
        {}
        
        explicit ThKey(ThI32 index)
            :
        m_Index(index)
        {}
        
        ThU32 Type()const
        {
            return TypeID;
        }
        
        ThU32 Kind()const
        {
            return KindID;
        }
        
        ThI32 Index()const
        {
            return m_Index;
        }
        
        ThTypeID<Class> GetType()const
        {
            return ThTypeID<Class>(TypeID, KindID);
        }
    private:
        ThI32 m_Index;
    };
    
    template <class Class, ThU32 KindID>
    class ThForeignKey
    {
    public:
        ThForeignKey()
            :
        m_Type(0, KindID),
        m_Uid(0)
        {}
        
        ThU32 TypeID()const
        {
            return m_Type.TypeID();
        }
        
        ThU32 Kind()const
        {
            return KindID;
        }
        
        ThU64 Uid()const
        {
            return m_Uid;
        }
    private:
        ThTypeID<Class> m_Type;
        ThU64 m_Uid;
    };
}