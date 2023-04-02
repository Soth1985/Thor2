#pragma once

/////////os dependent stuff
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
	#define WIN32_LEAN_AND_MEAN
	#define  THOR_PLATFORM_WIN
    #pragma warning(disable:4251)
    #pragma warning(disable:4275)
#endif

#ifdef __APPLE__
    #include "TargetConditionals.h"
    #define THOR_PLATFORM_APPLE
    #if TARGET_IPHONE_SIMULATOR
        #define THOR_PLATFORM_IOS
        #define THOR_PLATFORM_IOS_SIMULATOR
    #elif TARGET_OS_IPHONE
        #define THOR_PLATFORM_IOS
    #else
        #define THOR_PLATFORM_OSX
        #if TARGET_CPU_ARM64
            #define THOR_PLATFORM_OSX_ARM
        #elif TARGET_CPU_X86_64
            #define THOR_PLATFORM_OSX_X64
        #endif
    #endif
#endif

#include <assert.h>
#include <string>
#include <cstdint>

#ifdef THOR_PLATFORM_WIN
    #define THOR_INLINE __forceinline
#else
    #define THOR_INLINE inline
#endif

#ifndef NDEBUG
	#define THOR_DEBUG
#endif

#if defined(THOR_PLATFORM_WIN)
    #define THOR_ALIGNED(x) __declspec(align(x))
#else
    #define THOR_ALIGNED(x) __attribute__ ((aligned(x)))
#endif

#if INTPTR_MAX == INT64_MAX
    #define THOR_X64
#elif INTPTR_MAX == INT32_MAX
    #define THOR_X32
#else 
    #error "Unknown pointer size"
#endif

//////////////Configure Math Library
//#define USE_VECTOR_EXPRESSION_TEMPLATES
//#define USE_MATRIX_EXPRESSION_TEMPLATES

#if !defined(THOR_PLATFORM_IOS) && !defined(THOR_PLATFORM_OSX_ARM)
    #define THOR_ENABLE_SSE_MATH
#endif

#define THOR_CONCAT(x, y) x ## y
#define THOR_DEFAULT_ALIGNMENT 16

namespace Thor
{
	typedef float	 ThF32;
	typedef double	 ThF64;
    typedef char     ThChar;
	typedef int8_t	 ThI8;
	typedef uint8_t  ThU8;
	typedef int16_t	 ThI16;
	typedef uint16_t ThU16;
	typedef int32_t	 ThI32;
	typedef uint32_t ThU32;
	typedef int64_t	 ThI64;
	typedef uint64_t ThU64;
	typedef wchar_t	 ThWchar;
    typedef size_t   ThSize;

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

	static inline ThU32 NextPowerOf2(ThU32 val)
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

	static inline ThU64 NextPowerOf2(ThU64 val)
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

	static constexpr ThU64 Make8CC(const ThChar ext[9])
	{
		ThU64 result = 0;
		for (ThI32 i = 0; i < 8; ++i)
		{
			if (ext[i] == '\0')
			{
				break;
			}

			ThU64 letter = ext[i];
			letter = letter << (8 * (7 - i));
			result |= letter;
		}

		return result;
	}

	template <bool B>
	inline void STATIC_ASSERT_IMPL()
	{		
		char STATIC_ASSERT_FAILURE[B] = {0};
	}
    
    template <class T>
    using ValueType = typename T::value_type;

#ifdef THOR_PLATFORM_WIN
	#if _MSC_VER >= 1600
		#define THOR_STATIC_ASSERT(expr, desc) static_assert(expr, desc);
	#else
		#define THOR_STATIC_ASSERT(expr, desc) STATIC_ASSERT_IMPL <(expr)!=0>();
	#endif
#else
    #define THOR_STATIC_ASSERT(expr, desc) static_assert(expr, desc);
#endif

};//Thor
