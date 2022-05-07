#pragma once

#include <Thor/Core/Common.h>
#include <Thor/Core/Debug/ThAssert.h>

#define THOR_FORWARD_CLASS(className)	\
		class className;\
		typedef ThRefPtr<className> className##RefPtr;\
		typedef ThRefPtr<const className> className##RefCPtr;\
		typedef ThWeakPtr<className> className##WeakPtr;\
		typedef ThWeakPtr<const className> className##WeakCPtr;

#define THOR_FORWARD_STRUCT(structName)	\
		struct structName;\
		typedef ThRefPtr<structName> structName##RefPtr;\
		typedef ThRefPtr<const structName> structName##RefCPtr;\
		typedef ThWeakPtr<structName> structName##WeakPtr;\
		typedef ThWeakPtr<const structName> structName##WeakCPtr;

namespace Thor{

template<class T>
class ThDefaultActionPolicy
{
public:
	/*virtual*/ ~ThDefaultActionPolicy(){};
	void OnObjectDestroyed(){};
	
	void OnAssign(const T* rhs){};
	
	void OnCreateDefault(){};

	void OnCreateCopy(const T* rhs){};
};
///////////////////////////////////////////////////////////////////
/**	   
*	A COM like reference counting smart pointer. Thread safe with respect to a reference counter,
*	but releasing or assigning it to 0 in one thread whilst dereferencing in another is not recommended.
*/
template < class Type, template < typename> class ActionPolicy=ThDefaultActionPolicy >
class ThRefPtr:public ActionPolicy<Type>
{
private:
	typedef ThRefPtr SelfType;
public:

	typedef ActionPolicy<Type> ActionPolicyType;

	typedef Type ValueType;

	/**	   
	*	Returns true if pointer is not 0.
	*/

	bool operator!(void) const
	{
		return m_Pointee == 0;
	}

	operator bool() const
	{
		return m_Pointee != 0;
	}

	/**	   
	*	Returns the pointer to the stored object.
	*/

	friend inline Type* GetPointer(const ThRefPtr& lhs) 
	{
		return lhs.m_Pointee;
	}

	bool operator<(const ThRefPtr& rhs)const
	{
		return m_Pointee < rhs.m_Pointee;
	}
	
	template <class U, template <typename> class A>
	friend bool operator==(const ThRefPtr<Type, ActionPolicy>& lhs, const ThRefPtr<U,A>& rhs) 
	{
		return lhs.m_Pointee == GetPointer(rhs);
	}

	template <class U, template <typename> class A>
	friend bool operator!=(const ThRefPtr<Type, ActionPolicy>& lhs, const ThRefPtr<U,A>& rhs) 
	{
		return lhs.m_Pointee != GetPointer(rhs);
	}

	template <class U>
	friend bool operator==(const ThRefPtr<Type, ActionPolicy>& lhs, U* rhs) 
	{
		return lhs.m_Pointee == rhs;
	}

	template <class U>
	friend bool operator!=(const ThRefPtr<Type, ActionPolicy>& lhs, U* rhs) 
	{
		return lhs.m_Pointee != rhs;
	}

	template <class U>
	friend bool operator==(U* lhs, const ThRefPtr<Type, ActionPolicy>& rhs) 
	{
		return lhs == rhs.m_Pointee;
	}

	template <class U>
	friend bool operator!=(U* lhs, const ThRefPtr<Type, ActionPolicy>& rhs) 
	{
		return lhs != rhs.m_Pointee;
	}

	bool operator==(const ThRefPtr& rhs) const
	{
		return m_Pointee == rhs.m_Pointee;
	}

	bool operator!=(const ThRefPtr& rhs) const
	{
		return m_Pointee != rhs.m_Pointee;
	}

	void Swap(ThRefPtr& rhs)
	{
		Type* tmp = rhs.m_Pointee;
		rhs.m_Pointee = m_Pointee;
		m_Pointee = tmp;
	}

	ThRefPtr<Type>& operator=(Type* rhs)
	{
		if(m_Pointee != rhs)
			SelfType(rhs).Swap(*this);
		return *this;
	}
	
	template <class U, template <typename> class A>
	ThRefPtr<Type>& operator=(const ThRefPtr<U>& rhs)
	{
		Type* ptr = static_cast<Type*>( GetPointer(rhs) );
		if( ptr != m_Pointee )
			SelfType(rhs).Swap(*this);
		return *this;
	}
	
	ThRefPtr<Type>& operator=(const ThRefPtr<Type>& rhs)
	{
		if(m_Pointee != rhs.m_Pointee)
			SelfType(rhs).Swap(*this);
		return *this;
	}
	
	Type* operator->(void)
	{
		THOR_ASSERT( m_Pointee != 0, "Null pointer dereference");
		return m_Pointee;
	}

	const Type* operator->(void) const
	{
		THOR_ASSERT( m_Pointee != 0, "Null pointer dereference");
		return m_Pointee;
	}
	
	Type& operator*(void)
	{
		THOR_ASSERT( m_Pointee != 0, "Null pointer dereference");
		return *m_Pointee;
	}

	const Type& operator*(void) const
	{
		THOR_ASSERT( m_Pointee != 0, "Null pointer dereference");
		return *m_Pointee;
	}

	void AddRef()
	{		
		if(m_Pointee)
			m_Pointee->AddRef();
	}

	void AddRef()const
	{		
		if(m_Pointee)
			m_Pointee->AddRef();
	}	

	unsigned int GetRefCount() const
	{
		if(m_Pointee)
			return m_Pointee->GetRefCount();
		else 
			return 0;
	}

	void Release()
	{		
		if( m_Pointee )
		{
			if(m_Pointee->Release())
			{				
				m_Pointee = 0;
				ActionPolicyType::OnObjectDestroyed();
			}
		}		
	}

	void Discard()
	{
		m_Pointee = 0;
	}

	Type* GetPtr()
	{
		return m_Pointee;
	}

	const Type* GetPtr()const
	{
		return m_Pointee;
	}

	ThRefPtr()		
	{
		m_Pointee = 0;
		ActionPolicyType::OnCreateDefault();
	}

	~ThRefPtr()
	{
		Release();
	}

	ThRefPtr(Type* base)		
	{
		m_Pointee = base;
		AddRef();
		ActionPolicyType::OnCreateCopy(m_Pointee);
	}

	template <class U>
	ThRefPtr(U* base)
	{
		m_Pointee = static_cast<Type*>(base);
		AddRef();
		ActionPolicyType::OnCreateCopy(m_Pointee);
	}

	ThRefPtr(const ThRefPtr& base)		
	{
		m_Pointee = base.m_Pointee;
		AddRef();
		ActionPolicyType::OnCreateCopy(m_Pointee);
	}

	template <class U, template <typename> class A>
	ThRefPtr(const ThRefPtr<U, A>& base)
	{
		m_Pointee = static_cast<Type*>( GetPointer(base) );
		AddRef();
		ActionPolicyType::OnCreateCopy(m_Pointee);
	}

private:

	/**	   
	*	Pointer to the RefCounted object.
	*/
	Type* m_Pointee;
};

template <class TargetT, class SourceT, template <class> class ActionPolicyT = ThDefaultActionPolicy>
ThRefPtr<TargetT, ActionPolicyT> StaticCast(const ThRefPtr<SourceT, ActionPolicyT>& rhs)
{
	return ThRefPtr<TargetT, ActionPolicyT>(static_cast<TargetT*>(GetPointer(rhs)));
};

template <class TargetT, class SourceT, template <class> class ActionPolicyT = ThDefaultActionPolicy>
ThRefPtr<TargetT, ActionPolicyT> ReinterpretCast(const ThRefPtr<SourceT, ActionPolicyT>& rhs)
{
	return ThRefPtr<TargetT, ActionPolicyT>(reinterpret_cast<TargetT*>(GetPointer(rhs)));
};

template <class TargetT, class SourceT, template <class> class ActionPolicyT = ThDefaultActionPolicy>
ThRefPtr<TargetT, ActionPolicyT> ConstCast(const ThRefPtr<SourceT, ActionPolicyT>& rhs)
{
	return ThRefPtr<TargetT, ActionPolicyT>(const_cast<TargetT*>(GetPointer(rhs)));
};

template <class TargetT, class SourceT, template <class> class ActionPolicyT = ThDefaultActionPolicy>
ThRefPtr<TargetT, ActionPolicyT> DynamicCast(const ThRefPtr<SourceT, ActionPolicyT>& rhs)
{
	return ThRefPtr<TargetT, ActionPolicyT>(dynamic_cast<TargetT*>(GetPointer(rhs)));
};

template <class TargetT, class SourceT, template <class> class ActionPolicyT = ThDefaultActionPolicy>
ThRefPtr<TargetT, ActionPolicyT> CheckedCast(const ThRefPtr<SourceT>& rhs)
{
#ifdef _DEBUG
	ThRefPtr<TargetT, ActionPolicyT> tmp = (dynamic_cast<TargetT*>(GetPointer(rhs)));
	THOR_ASSERT(tmp != 0, "Checked cast failed");
	return tmp;
#else
	return ThRefPtr<TargetT, ActionPolicyT>( reinterpret_cast<TargetT*>( GetPointer(rhs) ) );
#endif
};

///////////////////////////////////////////////////////////////////
/**	   
*	Weak reference counter.
*/
class ThWeakCounter
{
public:

	void AddRef()
	{
		++m_Refcount;
	}

	bool Release()
	{
		if(--m_Refcount == 0)
		{
			delete this;
			return true;
		}
		else
			return false;
	}

	unsigned int GetRefCount(void)const
	{
		return m_Refcount;
	}

	bool IsValid()const
	{
		return m_IsValid;
	}

	void Invalidate()
	{
		m_IsValid=false;
	}

	~ThWeakCounter()
	{
	}

	ThWeakCounter(bool valid)
	{
		Init(valid);
	}

protected:	

	ThWeakCounter(const ThWeakCounter& rhs)
	{
		Init(false);
	}

	ThWeakCounter& operator=(const ThWeakCounter& copy)
	{
		return *this;
	}

private:

	void Init(bool valid)
	{
		m_Refcount = 0;
		m_IsValid = valid;
	}

	bool m_IsValid;

	/**	   
	*	Reference counter.
	*/
	#ifdef THOR_USE_TBB
		tbb::atomic<unsigned int> m_Refcount;
	#else
		unsigned int m_Refcount;
	#endif

};

///////////////////////////////////////////////////////////////////
/**	   
*	A COM like reference counting mechanism. Should be used as base class for reference counted objects.
*/
class ThRefCounted
{
public:	

	void AddRef()
	{
		++m_Refcount;
	}

	bool Release()
	{
		if(--m_Refcount == 0)
		{
			delete this;
			return true;
		}
		else
			return false;
	}

	unsigned int GetRefCount(void)const
	{
		return m_Refcount;
	}

	const ThRefPtr<ThWeakCounter>& GetWeakCounter()const
	{
		if( !m_Weak )
		{
			m_Weak=ThRefPtr<ThWeakCounter>( new ThWeakCounter(true) );						
		}		
		
		return m_Weak;
	}

protected:

	virtual ~ThRefCounted()
	{
		if( m_Weak )
			m_Weak->Invalidate();
	}

	ThRefCounted()
	{
		Init();
	}

	ThRefCounted(const ThRefCounted& copy)
	{
		Init();
	}

	ThRefCounted& operator=(const ThRefCounted& rhs)
	{
		return *this;
	}

private:

	void Init()
	{
		m_Weak = 0;
		m_Refcount = 0;
	}

	mutable ThRefPtr<ThWeakCounter> m_Weak;
	
	/**	   
	*	Reference counter.
	*/
	unsigned int m_Refcount;
};

template <class T>
class ThWeakPtr
{
public:
	typedef ThRefPtr<ThWeakCounter> CounterType;

	friend CounterType GetCounter(const ThWeakPtr& val)
	{
		return val.m_Counter;
	}
	
	ThWeakPtr& operator=(const ThWeakPtr& rhs)
	{
		InternalAssign(rhs);
		return *this;
	}	

	template<template <class> class A>
	ThWeakPtr& operator=(const ThRefPtr<T, A>& rhs)
	{
		InternalAssign(rhs);
		return *this;
	}

	ThRefPtr<T> Lock()const
	{
		if(m_Counter && m_Counter->IsValid())
			return ThRefPtr<T>(m_Pointee);
		else
			return ThRefPtr<T>(0);
	}	

	ThWeakPtr()
		:
	m_Pointee(0),
	m_Counter(0)
	{
		//
	}	

	template<class U, template <class> class A>
	ThWeakPtr(const ThRefPtr<U, A>& ptr)
	{
		ThRefPtr<T, A> tmp = DynamicCast<T>(ptr);
		InternalAssignRefPtr(tmp);
	}

	template<template <class> class A>
	ThWeakPtr(const ThRefPtr<T, A>& ptr)
	{
		InternalAssignRefPtr(ptr);
	}

	ThWeakPtr(const ThWeakPtr& ptr)
	{
		InternalAssign(ptr);
	}

	template<class U>
	ThWeakPtr(const ThWeakPtr<U>& ptr)
	{
		InternalAssign(ptr);
	}
private:

	void InternalAssign(const ThWeakPtr& ptr)
	{
		m_Pointee = GetPointer( ptr.Lock() );
		m_Counter = GetCounter(ptr);
	}

	template<template <class> class A> 
	void InternalAssignRefPtr(const ThRefPtr<T, A>& ptr)
	{
		if( !ptr )
		{
			m_Pointee = 0;
			m_Counter = 0;
		}
		else
		{
			m_Pointee = static_cast<T*>( GetPointer(ptr) );
			m_Counter = ptr->GetWeakCounter();
		}
	}

	CounterType m_Counter;
	T* m_Pointee;
};


}//Thor