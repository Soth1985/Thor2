#pragma once

/////////os dependent stuff
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
	#define WIN32_LEAN_AND_MEAN
	//#include <windows.h>
	#define  THOR_MS_WIN
    #pragma warning(disable:4251)
    #pragma warning(disable:4275)
#endif

#include <assert.h>
#include <string>

#ifdef THOR_MS_WIN
    #define THOR_INLINE __forceinline
#else
    #define THOR_INLINE inline
#endif

#ifdef _DEBUG
	#define THOR_DEBUG
#endif

#if defined(THOR_MS_WIN)
    #define THOR_ALIGNED(x) __declspec(align(x))
#else
    #define THOR_ALIGNED(x) __attribute__ ((aligned(x)))
#endif

//////////////Configure Math Library
//#define USE_VECTOR_EXPRESSION_TEMPLATES
//#define USE_MATRIX_EXPRESSION_TEMPLATES

#ifdef THOR_X32
	#define USE_SSE_MATH
#endif

#define THOR_CONCAT(x, y) x ## y

//#define THOR_USE_TBB
//#define __TBB_NO_IMPLICIT_LINKAGE

namespace Thor
{
	typedef float				ThF32;
	typedef double				ThF64;
	typedef char				ThI8;
	typedef unsigned char		ThU8;
	typedef short				ThI16;
	typedef unsigned short		ThU16;
	typedef int					ThI32;	
	typedef unsigned int		ThU32;
	typedef long long			ThI64;
	typedef unsigned long long	ThU64;
	typedef wchar_t				ThWchar;
	typedef size_t				ThSize;
	typedef std::string			ThString;
	typedef std::wstring		ThWideString;
	typedef std::u16string		ThU16String;
	
	class ThBool
	{
	public:
		ThBool()
			:m_Val(0)
		{};

		ThBool( bool val_ )
			:m_Val(val_)
		{};

		operator ThI32()const
		{
			return ThI32(m_Val);
		}

		operator bool () const
		{
			return m_Val != 0;
		}

		ThU8& operator=(bool rhs)
		{
			m_Val = rhs;
			return m_Val;
		}

		bool operator==(bool rhs)const
		{
			return m_Val == ThU8(rhs);
		}

		bool operator!=(bool rhs)const
		{
			return m_Val != ThU8(rhs);
		}

		bool operator==(const ThBool rhs)const
		{
			return m_Val == rhs.m_Val;
		}

		bool operator!=(const ThBool rhs)const
		{
			return m_Val != rhs.m_Val;
		}

	private:
		ThU8 m_Val;
	};

	class NonCopyable
	{
	public:
		NonCopyable(){}
	private:

		NonCopyable(const NonCopyable& copy){}
		NonCopyable& operator=(const NonCopyable& rhs){return *this;}
	};

	template <class FirstT, class SecondT>
	class ThPair
	{
	public:

		typedef FirstT FirstType;
		typedef SecondT SecondType;

		ThPair()
		{

		}

		ThPair(const FirstT& first, const SecondT& second)
			:
		m_First(first),
		m_Second(second)
		{

		}

		const FirstT& First()const
		{
			return m_First;
		}

		FirstT& First()
		{
			return m_First;
		}

		const SecondT& Second()const
		{
			return m_Second;
		}

		SecondT& Second()
		{
			return m_Second;
		}

	private:

		FirstT m_First;
		SecondT m_Second;
	};

	template < class T >
	const T& Max( const T& a, const T& b )
	{
		if( a < b )
			return b;
		else 
			return a;
	}

	template < class T >
	T& Min( T& a, T& b )
	{
		if( a < b )
			return a;
		else 
			return b;
	}

	template < class T >
	T& Max( T& a, T& b )
	{
		if( a < b )
			return b;
		else 
			return a;
	}

	template < class T >
	const T& Min( const T& a, const T& b )
	{
		if( a < b )
			return a;
		else 
			return b;
	}

	template < class T >
	void Swap( T& a, T& b )
	{
		T temp = a;
		a = b;
		b = temp;
	}

	template < class Type >
	ThU32 NextID()
	{
		static ThU32 id = 0;
		return id++;
	};

	static ThU32 NextPowerOf2(ThU32 val)
	{
		val--;

		val |= (val >> 1);
		val |= (val >> 2);
		val |= (val >> 4);
		val |= (val >> 8);
		val |= (val >> 16);
		
		val++;

		return val;
	}

	static ThU64 NextPowerOf2(ThU64 val)
	{
		val--;

		val |= (val >> 1);
		val |= (val >> 2);
		val |= (val >> 4);
		val |= (val >> 8);
		val |= (val >> 16);
		val |= (val >> 32);

		val++;

		return val;
	}

	template <bool B>
	inline void STATIC_ASSERT_IMPL()
	{		
		char STATIC_ASSERT_FAILURE[B] = {0};
	}

#ifdef THOR_MS_WIN
	#if _MSC_VER >= 1600
		#define THOR_STATIC_ASSERT(expr, desc) static_assert(expr, desc);
	#else
		#define THOR_STATIC_ASSERT(expr, desc) STATIC_ASSERT_IMPL <(expr)!=0>();
	#endif
#else
    #define THOR_STATIC_ASSERT(expr, desc) static_assert(expr, desc);
#endif

};//Thor