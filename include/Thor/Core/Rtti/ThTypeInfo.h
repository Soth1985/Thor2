#pragma once

#include <string>
#include <unordered_set>
#include <set>
#include <functional>
#include <type_traits>
#include <Thor/Core/Common.h>
#include <Thor/Core/TypeLists.h>
#include <Thor/Core/Rtti/ThRttiManager.h>
#include <Thor/Core/Containers/ThHashSet.h>

namespace Thor{

template <class T>
struct ThClassName;

#define THOR_DECL_TYPE(t) \
	template<> struct Thor::ThClassName<t>\
	{\
	static const ThI8* GetName()\
	{\
		return #t;\
	}\
	};

    
template<class T>
ThiType* TypeOf();
    
template <class T>
class ThType;
    
namespace Private
{
    template<class T, class Parent>
    struct ProcessParents;
}//Private
//----------------------------------------------------------------------------------------
//
//					ThiType
//
//----------------------------------------------------------------------------------------
class THOR_FRAMEWORK_DLL ThiType
{
public:
	virtual const ThI8* GetName()const=0;	
	virtual ThBool		IsBaseOf(const ThiType* c)const=0;
	virtual ThBool		IsChildOf(const ThiType* c)const=0;
	virtual ThBool		IsAbstract()const=0;
	virtual ThSize		GetNumBases()const=0;
	virtual const ThiType*	GetBaseAt(ThSize i)const=0;
	virtual ThSize		GetNumChildren()const=0;
	virtual const ThiType*	GetChildAt(ThSize i)const=0;
	virtual ThBool		IsFundamental()const=0;
	virtual ThU32		GetSize()const=0;

	template<class T>
	T* CreateObject(ThU32 param = 0)
	{
		ThiType* type = TypeOf<T>();
		if(this == type || this->IsChildOf(type) )
		{
			return (T*)CreateObject(param);
		}
		else
			return 0;
	}

	virtual void* CreateObject(ThU32 param = 0) = 0;

	virtual ~ThiType()
	{
		//
	}

	

private:
	friend class ThRttiManager;
	
    template <class T>
	friend class ThType;
    
    template<class T, class Parent>
    friend struct Private::ProcessParents;
	
	virtual void RemoveType(const ThiType* t)=0;
	virtual void AddChild(const ThiType* t)=0;
	virtual void AddBase(const ThiType* t)=0;
};
    
namespace Private
{
    //----------------------------------------------------------------------------------------
    //
    //					CreateClassInstance
    //
    //----------------------------------------------------------------------------------------
    template<class T, bool abstr>
    struct CreateClassInstance;
    
    template<class T>
    struct CreateClassInstance<T, true>
    {
        static void* Create(ThU32 param)
        {
            return 0;
        }
    };
    
    template<class T>
    struct CreateClassInstance<T, false>
    {
        static void* Create(ThU32 param)
        {
            return new T();
        }
    };
    
    template<class T, class Parent>
    struct ProcessParents
    {
        static void Process()
        {
            THOR_STATIC_ASSERT( (std::is_base_of<Parent, T>::value), "These classes are not related" );
            ThiType* parent = ThType<Parent>::Instance();
            ThiType* thisType = ThType<T>::Instance();
            
            parent->AddChild(thisType);
            thisType->AddBase(parent);
        }
    };
    
    template<class T>
    struct ProcessParents<T, NullType>
    {
        static void Process()
        {
            
        }
    };
    
    template<class T>
    struct ProcessParents<NullType, T>
    {
        static void Process()
        {
            
        }
    };
    
    template<>
    struct ProcessParents<NullType, NullType>
    {
        static void Process()
        {
            
        }
    };
    
    template<class T, class Head, class Tail>
    struct ProcessParents< T, Typelist<Head, Tail> >
    {
        static void Process()
        {
            typedef typename TypeAt<Typelist<Head, Tail>, 0>::Result CurParent;
            ProcessParents<T, CurParent>::Process();
            ProcessParents<CurParent, Tail>::Process();
        }
    };
    
}//Private
//----------------------------------------------------------------------------------------
//
//					ThType
//
//----------------------------------------------------------------------------------------
template <class T>
class ThType:public ThiType, public NonCopyable
{
public:	

	virtual const ThI8* GetName()const
	{
		return ThClassName<T>::GetName();
	}	

	virtual ThBool IsBaseOf(const ThiType* c)const
	{
		ClassList::ConstIterator i = m_Children.Find(c);
		return i != m_Children.End();		
	}

	virtual ThBool IsChildOf(const ThiType* c)const
	{
		ClassList::ConstIterator i = m_Bases.Find(c);
		return i != m_Bases.End();
	}

	virtual ThBool IsAbstract()const
	{
        return std::is_abstract<T>::value;
	}

	virtual ThSize GetNumBases()const
	{
		return m_Bases.Size();
	}

	virtual const ThiType* GetBaseAt(ThSize i)const
	{
		if( i < m_Bases.Size() )
		{
			ThSize j = 0;
			ClassList::ConstIterator t = m_Bases.Begin();

			for(; j < i; ++t,++j){};
			return t->Key();
		}

		return 0;
	}

	virtual ThSize GetNumChildren()const
	{
		return m_Children.Size();
	}

	virtual const ThiType* GetChildAt(ThSize i)const
	{
		if( i < m_Children.Size() )
		{
			ThSize j = 0;
			ClassList::ConstIterator t = m_Children.Begin();

			for(; j < i; ++t,++j){};
			return t->Key();
		}

		return 0;
	}

	virtual ThBool IsFundamental()const
	{
		return std::is_fundamental<T>::value;
	}

	virtual ThU32 GetSize()const
	{
		return sizeof(T);
	}	

	virtual void RemoveType(const ThiType* t)
	{
		m_Bases.Erase(t);
		m_Children.Erase(t);

		/*for(ClassList::Iterator i = m_Bases.Begin(); i != m_Bases.End(); ++i)
		{
			i->Key()->RemoveType(t);
		}

		for(ClassList::Iterator i = m_Children.Begin(); i != m_Children.End(); ++i)
		{
			i->Key()->RemoveType(t);
		}*/
	}

	~ThType()
	{
		if (m_Self == &SingletonT::Instance())//unregister the type if it is located in this dll
		{
			ThRttiManager::Instance().UnregisterType(m_Self);
		}
	}

	static ThiType* Instance()
	{
		if( !m_Self )
		{			
			m_Self = ThRttiManager::Instance().GetType( ThClassName<T>::GetName() );

			if( !m_Self )
			{
				ThiType* thisInst = &SingletonT::Instance();
				ThRttiManager::Instance().RegisterType(thisInst);
				m_Self = thisInst;
				return thisInst;
			}
			else
				return m_Self;
		}
		else
			return m_Self;
	}	

	template<class Parents>
    static void SetParents()
	{
        Private::ProcessParents<T, Parents>::Process();
	}

	virtual void* CreateObject(ThU32 param)
	{
		return Private::CreateClassInstance<T, std::is_abstract<T>::value>::Create(param);
	}
private:	

	ThType()
	{		
				
	}

	virtual void AddChild(const ThiType* t)
	{
		m_Children.Insert(t);

		for(size_t Idx = 0; Idx < t->GetNumChildren(); ++Idx)
		{
			m_Children.Insert(t->GetChildAt(Idx));
		}

		for (ClassList::Iterator i = m_Bases.Begin(); i != m_Bases.End(); ++i)
		{
			ThiType* type = const_cast<ThiType*>(i->Key());
			type->AddChild(t);
		}
	}

	virtual void AddBase(const ThiType* t)
	{
		m_Bases.Insert(t);

		for (ThSize i = 0; i < t->GetNumBases(); ++i)
		{
			m_Bases.Insert(t->GetBaseAt(i));
		}

		for (ClassList::Iterator i = m_Children.Begin(); i != m_Children.End(); ++i)
		{
			ThiType* curType = const_cast<ThiType*>(i->Key());
			curType->AddBase(t);
		}
	}

	template<class Ty, class Parents>
	friend struct RegisterType;
	
	typedef SingletonWithLongevity<ThType, eSingletonsLongevity::TypeInfo> SingletonT;
	friend SingletonT;
	typedef ThHashSet<const ThiType*> ClassList;

	ClassList	m_Bases;
	ClassList	m_Children;	
	static ThiType*	m_Self;
};

template <class T>
ThiType* ThType<T>::m_Self = 0;
//----------------------------------------------------------------------------------------
//
//					RegisterType
//
//----------------------------------------------------------------------------------------
template<class Ty, class Parents>
struct RegisterType
{
	RegisterType()
	{
        ThType<Ty>::Instance();
        ThType<Ty>::template SetParents<Parents>();
	}	
};
//----------------------------------------------------------------------------------------
template<class Ty>
struct RegisterType< Ty, THOR_TYPELIST_0() >
{
	RegisterType()
	{		
		ThType<Ty>::Instance();
	}	
};
//----------------------------------------------------------------------------------------
template<class T>
ThBool IsOfType(ThiType* t)
{
	return t == ThType<T>::Instance();
}
//----------------------------------------------------------------------------------------
template<class T>
ThiType* TypeOf()
{
	return ThType<T>::Instance();
}
//----------------------------------------------------------------------------------------
static inline ThBool IsConvertible(ThiType* from, ThiType* to)
{
	if( from == to || to->IsBaseOf(from) || to->IsChildOf(from) )
	{
		return true;
	}
	else
		return false;
}
//----------------------------------------------------------------------------------------
template <class TgtT, class SrcT>
TgtT* TypeCast(SrcT* src)
{
	TgtT* result = 0;

	if(src)
	{
		ThiType* srcType = src->GetType();
		ThiType* tgtType = ThType<TgtT>::Instance();

		if( IsConvertible(srcType, tgtType) )
		{
			result = static_cast<TgtT*>(src);
		}
	}

	return result;
}
//----------------------------------------------------------------------------------------
template <class TgtT, class SrcT>
const TgtT* TypeCast(const SrcT* src)
{
	const TgtT* result=0;

	if(src)
	{
		ThiType* srcType = src->GetType();
		ThiType* tgtType = ThType<TgtT>::Instance();

		if( IsConvertible(srcType, tgtType) )
		{
			result = static_cast<TgtT*>(src);
		}
	}

	return result;
}
//----------------------------------------------------------------------------------------
#define THOR_REG_ROOT_TYPE(t) static Thor::RegisterType<t, THOR_TYPELIST_0()> thor_rtti_##t;
#define THOR_REG_TYPE(t, parents) static Thor::RegisterType<t, parents> thor_rtti_##t;

}//Thor