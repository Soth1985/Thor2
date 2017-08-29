#pragma once

#include <Thor/Core/Common.h>
#include <Thor/Core/SmartPointers/RefPtr.h>
#include <Thor/Core/TypeLists.h>

namespace Thor
{

class ThiObject;
typedef ThRefPtr<ThiObject> ThiObjectPtr;
//----------------------------------------------------------------------------------------
//
//					ObjectCast
//
//----------------------------------------------------------------------------------------
template <class SrcT, class TgtT, template < typename> class SrcActionPolicyT, template < typename> class TgtActionPolicyT>
ThRefPtr< TgtT, TgtActionPolicyT > ObjectCast(const ThRefPtr< SrcT, SrcActionPolicyT>& src)
{
	typedef ThRefPtr< TgtT, TgtActionPolicyT > res_t;
	
	TgtT* result = TypeCast<TgtT>( GetPointer(src) );	

	return res_t(result);
}
//----------------------------------------------------------------------------------------
template <class TgtT, class SrcT>
ThRefPtr< TgtT > ObjectCast(const ThRefPtr<SrcT>& src)
{
    return ObjectCast<SrcT, TgtT,  ThDefaultActionPolicy, ThDefaultActionPolicy>(src);
}
//----------------------------------------------------------------------------------------
//
//					StaticVisitor
//
//----------------------------------------------------------------------------------------
template<class TypeT, template < typename> class ActionPolicyT = ThDefaultActionPolicy >
struct StaticVisitor
{
	template<class VisitorT>
	static void Visit(ThiObjectPtr& obj, VisitorT& visitor, bool downCast = false)
	{	
		typedef ThRefPtr<TypeT, ActionPolicyT> ptr_t;
		ThiType* visType = TypeOf<TypeT>();
		ThiType* objType = obj->GetType();

		bool visit = ( visType == objType );
		//if not an exact match, try downcasting
		if (!visit && downCast)
		{
			visit = visType->IsBaseOf(objType);
		}

		if (visit)
		{
			ThiObject* pObj = GetPointer(obj);
			TypeT* ptr = static_cast<TypeT*>(pObj);
			visitor.Visit( ptr_t(ptr) );
		}		
	}
};
//----------------------------------------------------------------------------------------
template< template < typename> class ActionPolicyT >
struct StaticVisitor<NullType, ActionPolicyT>
{
	template<class VisitorT>
	static void Visit(ThiObjectPtr& obj, VisitorT& visitor, bool downCast = false)
	{
		//no op
	}
};
//----------------------------------------------------------------------------------------
template<class Head, class Tail>
struct StaticVisitor< Typelist<Head, Tail>, ThDefaultActionPolicy >
{
	template<class VisitorT>
	static void Visit(ThiObjectPtr& obj, VisitorT& visitor, bool downCast = false)
	{
		StaticVisitor<Head, ThDefaultActionPolicy>::Visit(obj, visitor, downCast);
		StaticVisitor<Tail, ThDefaultActionPolicy>::Visit(obj, visitor, downCast);
	}	
};
//----------------------------------------------------------------------------------------
template<class Head, class Tail, template < typename> class ActionPolicyT>
struct StaticVisitor< Typelist<Head, Tail>, ActionPolicyT >
{
	template<class VisitorT>
	static void Visit(ThiObjectPtr& obj, VisitorT& visitor, bool downCast = false)
	{
		StaticVisitor<Head, ActionPolicyT>::Visit(obj, visitor, downCast);
		StaticVisitor<Tail, ActionPolicyT>::Visit(obj, visitor, downCast);
	}	
};

}
