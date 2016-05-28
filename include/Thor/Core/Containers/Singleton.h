#pragma once

#include <utility>
#include <vector>
#include <algorithm>
#include <Thor/Core/Memory/ThMemory.h>
#include <Thor/Core/Common.h>

#ifdef THOR_USE_TBB
#include <tbb/spin_mutex.h>
#endif

namespace Thor
{ 

/*!
 * \brief
 * This class represents a simple single instance object.
 * 
 * \param T
 * T is the type for Singleton to wrap. It should hide default and copy constructors,
 * assignment operator.
 *
 * Singleton objects are destroyed in an unspecified order when the program shuts down.
 * To use Singleton with your class you should declare Singleton as this class`s friend.
 *
 */
template <class T>
class Singleton
{
	
	template<class KillT>
	class Destroyer
	{
		KillT* m_Ptr;
	public:
		Destroyer():m_Ptr(0){};

		~Destroyer()
		{
			if (m_Ptr)
			{
				m_Ptr->~KillT();
				ThMemory::Free(m_Ptr);
				m_Ptr = 0;
			}
		}

		void SetPtr( KillT* p )
		{
			m_Ptr = p;
		}
	};
	
	
	//friend class Destroyer<T>;
protected:
	static T* m_Object;
	typedef Destroyer<T> destroyer_t;
	static destroyer_t destroyer;
#ifdef THOR_USE_TBB
	typedef tbb::spin_mutex mutex_t;
	static mutex_t m_Mutex;
#endif
	
public:
	/*!
	 * \brief
	 * Returns reference to the stored object.
	 * 
	 * \returns
	 * Returns reference to the stored object.
	 */
	static T& Instance()
	{
#ifdef THOR_USE_TBB
		if (!m_Object)
		{
			mutex_t::scoped_lock lock(m_Mutex);

			if (!m_Object)
			{
				m_Object = New();
				destroyer.SetPtr( m_Object );	
			}				
		}		

		return *m_Object;
#else

		if (!m_Object)
		{
			m_Object = New();			
			destroyer.SetPtr( m_Object );				
		}
		//static T object;
		return *m_Object;
#endif
	}
protected:

	static T* New()
	{
		void* ptr = ThMemory::Malloc( sizeof (T) );
		T* val = (T*)ptr;
		new(ptr) T();
		return val;
	}

	Singleton(){};
	Singleton( const Singleton& copy ){};
	~Singleton(){};
};

template<class T>
T* Singleton<T>::m_Object=0;

#ifdef THOR_USE_TBB
	template<class T>
	tbb::spin_mutex Singleton<T>::m_Mutex;
#endif

template<class T>
typename Singleton<T>::destroyer_t Singleton<T>::destroyer;

namespace Private
{
	/*!
	 * \brief
	 * A base class for all LongevityObject classes.
	 *
	 * LongevityManager stores a list of pointers to singletons wrapped into LongevityObject
	 * to ensure their deletion in required order.
	 */
	class LongevityObjectBase
	{
	public:
		virtual void Release() = 0;
		virtual ~LongevityObjectBase(){};
	};

	/*!
	 * \brief
	 * Every SingletonWithLongevity creates an object of this class.
	 *
	 * \param SingletonT
	 * A singleton template parameter.
	 *
	 * 
	 */
	template< class SingletonT >
	class LongevityObject: public LongevityObjectBase
	{
		typename SingletonT::value_type* m_Ptr;

	public:

		virtual void Release()
		{
			SingletonT::Delete(m_Ptr);
		};

		LongevityObject( typename SingletonT::value_type* ptr_ )
			:m_Ptr(ptr_)
		{}

	};
	/*!
	 * \brief
	 * Manages deletion of SingletonWithLongevity objects.
	 *
	 * When this class is destroyed it traverses a list of registered singletons and deletes them in required order.
	 * 
	 */
	class THOR_FRAMEWORK_DLL LongevityManager
	{
		
		typedef std::pair<unsigned int, LongevityObjectBase* > pair_t;
		typedef std::vector<pair_t> list_t;
		/*!
		 * \brief
		 * A sorted list of SingletonWithLongevity objects.
		 */
		list_t longevityObj;

		static bool Less( const pair_t& a, const pair_t& b );

		typedef Singleton<LongevityManager> Singleton;

		
	public:

		//static LongevityManager longevityManager;
		/*!
		 * \brief
		 * Registers a SingletonWithLongevity object in the list.
		 *
		 * \param longevity
		 * Singleton longevity. Lower value means earlier deletion.
		 * \param ptr
		 * Pointer to a singleton object.
		 */
		void Register( unsigned int longevity, LongevityObjectBase* ptr );

		static LongevityManager& Instance(); 

		~LongevityManager();
	};

	
}//Private

	/*!
	 * \brief
	 * This class represents a single instance object with longevity.
	 * 
	 * \param T
	 * T is the type for Singleton to wrap. It should hide default and copy constructors,
	 * assignment operator and destructor.
	 *
	 * \param longevity
	 * Specifies the lifetime of the object.
	 *
	 * When the program shuts down SingletonWithLongevity objects are destroyed in order,
	 * specified by the longevity parameter,
	 * objects with lower longevity are destroyed earlier.
	 * To use SingletonWithLongevity with your class you should declare SingletonWithLongevity as  as this class`s friend, and
	 * if needed add Thor/Utility/Singleton.cpp to your project`s source files.
	 */
	template <class T, unsigned int longevity>
	class SingletonWithLongevity
	{
		friend class Private::LongevityObject<SingletonWithLongevity>;
	protected:
		/*!
		 * \brief
		 * Pointer to singleton object.
		 */
		static T* m_Object;
		/*!
		 * \brief
		 * Creates singleton instance object.
		 * 
		 * \returns
		 * Pointer to an object.
		 */
		static T* Create()
		{		
			void* ptr = ThMemory::Malloc( sizeof(T) );
			T* val = (T*)ptr;
			new(ptr) T();	
			return val;
		};
		/*!
		 * \brief
		 * Destroys singleton.
		 * 
		 * \param ptr
		 * Pointer to an object.
		 */
		static void Delete(T* ptr)
		{
			ptr->~T();
			ThMemory::Free(ptr);
		};

#ifdef THOR_USE_TBB
		typedef tbb::spin_mutex mutex_t;
		static mutex_t m_Mutex;
#endif

	public:
		typedef T value_type;
		/*!
		 * \brief
		 * Returns reference to the stored object.
		 * 
		 * \returns
		 * Returns reference to the stored object.
		 */
		static T& Instance()
		{
#ifdef THOR_USE_TBB
			if (!m_Object)
			{
				mutex_t::scoped_lock lock(m_Mutex);

				if (!m_Object)
				{
					m_Object = Create();
					Private::LongevityManager::Instance().Register(longevity, new Private::LongevityObject<SingletonWithLongevity>(m_Object) );
				}				
			}			

			return *m_Object;
#else
			if (!m_Object)
			{
				m_Object = Create();
				Private::LongevityManager::Instance().Register(longevity, new Private::LongevityObject<SingletonWithLongevity>(m_Object) );						
			}

			return *m_Object;
#endif
		}
	protected:
		SingletonWithLongevity(){};
		SingletonWithLongevity( const SingletonWithLongevity& copy ){};
		~SingletonWithLongevity(){};
	};

	template<class T, unsigned int longevity>
	T* SingletonWithLongevity<T, longevity>::m_Object = 0;

#ifdef THOR_USE_TBB
	template<class T, unsigned int longevity>
	tbb::spin_mutex SingletonWithLongevity<T, longevity>::m_Mutex;
#endif


}//Thor