#pragma once

#include <Thor/Core/Common.h>
#include <Thor/Core/Containers/ThHash.h>

namespace Thor
{
    struct eThType
    {
        enum Val
        {
            AnyType = 1
        };
    };

    template <class Class>
    class ThTypeID
    {
    public:
        explicit ThTypeID(ThU32 kind, ThU32 type)
            :
        m_Type(type),
        m_Kind(kind)
        {}
        
        operator bool()const
        {
            return Id() != 0;
        }
        
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
        
        ThU32 Id()const
        {
            return (m_Type << 10) | m_Kind;
        }
        
    private:
        ThU32 m_Type : 22;
        ThU32 m_Kind : 10;
    };
    
    template <class Class>
    struct ThHash< ThTypeID<Class> >
    {
        static inline ThSize HashCode(const ThTypeID<Class>& type)
        {
            return Private::HashFunc(type.Id());
        }
    };
    
    template <class Class, ThU32 TypeID, ThU32 KindID>
    class ThInstance
    {
    public:
        ThInstance()
            :
        m_Index(-1)
        {}
        
        explicit ThInstance(ThI32 index)
            :
        m_Index(index)
        {}
        
        operator bool()const
        {
            return m_Index >= 0;
        }
        
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
    
    template <class Class, ThU32 TypeID, ThU32 KindID>
    struct ThHash< ThInstance<Class, TypeID, KindID> >
    {
        static inline ThSize HashCode(const ThInstance<Class, TypeID, KindID>& key)
        {
            return Private::HashFunc(key.Index());
        }
    };
    
    template <class Class, ThU32 KindID>
    class ThKey
    {
    public:
        ThKey(ThU32 type)
        :
        m_Uid(0),
        m_Type(KindID, type)
        {}
        
        explicit ThKey(ThU64 uid, ThU32 type)
        :
        m_Uid(uid)
        {}
        
        operator bool()const
        {
            return m_Uid != 0;
        }
        
        ThU32 Type()const
        {
            return m_Type.Type();
        }
        
        ThU32 Kind()const
        {
            return m_Type.Kind();
        }
        
        ThU64 Uid()const
        {
            return m_Uid;
        }
    private:
        ThU64 m_Uid;
        ThTypeID<Class> m_Type;
    };
    
    template <class Class, ThU32 KindID>
    struct ThHash< ThKey<Class, KindID> >
    {
        static inline ThSize HashCode(const ThKey<Class, KindID>& key)
        {
            return Private::HashFunc(key.Uid());
        }
    };
}
