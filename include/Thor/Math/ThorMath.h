#pragma once

#include <Thor/Core/Common.h>
#include <Thor/Math/Constants.h>
#include <Thor/Math/FixedVectorFwd.h>
#include <math.h>

namespace Thor{
	namespace Math{

template < class RealT >
THOR_INLINE RealT DegToRad( const RealT& angle )
{
	return angle * RealT(Pi / 180.0);
}

template <class RealT, class IntT>
RealT Factorial(const IntT& k)
{
	if ((k == 0) || (k == 1))
		return 1;

	IntT i;
	IntT result = 2;

	for(i = 3;i <= k;i++) 
		result *= i;

	return result;	

}
//////////////////////////////////////////////////////////////////////////
template <class RealT, class IntT>
RealT Bin(const IntT& k,const IntT& i)
{
	IntT j;
	RealT result;

	if( (i == 0) || (i == k) )
		return 1.0;

	result = (RealT) k;

	for(j = 1;j < i;j++)
	{
		result *= (k - j);
		result /= j;
	}

	return result / j;
}

template < class RealT >
THOR_INLINE RealT RadToDeg( const RealT& angle )
{
	return angle * RealT(180.0 / Pi);
}

#pragma region Math functions with single values and vectors

template <class RealT>
THOR_INLINE RealT Sqrt(const RealT& f)
{
	return sqrt(f);
}

template <class VecT>
THOR_INLINE void SqrtvOp(const VecT& arg, VecT& res)
{
	for( unsigned int i = 0; i < arg.Size(); ++i )
		res(i) = sqrt( arg(i) );
}

template <class RealT>
THOR_INLINE RealT InvSqrt(const RealT& f)
{
	return RealT(1.0) / sqrt(f);
}

template <class VecT>
THOR_INLINE void InvSqrtvOp(const VecT& arg , VecT& res)
{
	typedef typename VecT::value_type value_type;
	for( unsigned int i = 0; i < arg.Size(); ++i )
		res(i) = value_type(1.0) / sqrt( arg(i) );	
}

template <class RealT>
THOR_INLINE RealT Sin(const RealT& f)
{
	return	sin(f);
}

template <class VecT>
THOR_INLINE void SinvOp(const VecT& arg , VecT& res)
{
	for( unsigned int i = 0; i < arg.Size(); ++i )
		res(i) = sin( arg(i) );
}

template <class RealT>
THOR_INLINE RealT Cos(const RealT& f)
{
	return	cos(f);
}

template <class VecT>
THOR_INLINE void CosvOp(const VecT& arg , VecT& res)
{
	for( unsigned int i = 0; i < arg.Size(); ++i )
		res(i) = cos( arg(i) );
}

template < class RealT >
THOR_INLINE void SinCos(const RealT& f, RealT &s, RealT &c)
{
	s = sin(f);
	c = cos(f);
}

template < class RealT >
THOR_INLINE RealT Cotan(const RealT& f)
{
	return RealT(1.0) / tan(f);
}

template <class VecT>
THOR_INLINE void CotanvOp(const VecT& arg , VecT& res)
{
	typedef typename VecT::value_type value_type;
	for( unsigned int i = 0; i < arg.Size(); ++i )
		res(i) = value_type(1.0) / tan( arg(i) );
}

template < class RealT >
THOR_INLINE RealT Tan(const RealT& f)
{
	return tan(f);
}

template <class VecT>
THOR_INLINE void TanvOp(const VecT& arg , VecT& res)
{
	for( unsigned int i = 0; i < arg.Size(); ++i )
		res(i) = tan( arg(i) );
}

template <class VecT>
THOR_INLINE void SinCosv(const VecT& arg, VecT& s, VecT& c)
{
	for( unsigned int i = 0; i < arg.Size(); ++i )
	{
		s(i) = sin( arg(i) );
		c(i) = cos( arg(i) );
	}
}
template < class RealT >
THOR_INLINE RealT Atan( const RealT& f ) 
{
	return	atan(f);
}

template <class VecT>
THOR_INLINE void AtanvOp(const VecT& arg , VecT& res)
{
	for( unsigned int i = 0; i < arg.Size(); ++i )
		res(i) = atan( arg(i) );
}

template < class RealT >
THOR_INLINE RealT Atan2( const RealT& y, const RealT& x )
{
	return	atan2(y,x);
}

template <class VecT>
THOR_INLINE void Atan2vOp(const VecT& y, const VecT& x, VecT& res)
{	
	for( unsigned int i = 0; i < res.Size(); ++i )
		res(i) = atan2( y(i), x(i) );
}

template < class RealT >
THOR_INLINE RealT Asin( const RealT& f)
{
	return	asin(f);
}

template <class VecT>
THOR_INLINE void AsinvOp(const VecT& arg , VecT& res)
{
	for( unsigned int i = 0; i < arg.Size(); ++i )
		res(i) = asin( arg(i) );
}

template < class RealT >
THOR_INLINE RealT	Acos( const RealT& f)
{
	return	acos(f);
}

template <class VecT>
THOR_INLINE void AcosvOp(const VecT& arg , VecT& res)
{
	for( unsigned int i = 0; i < arg.Size(); ++i )
		res(i) = acos( arg(i) );
}

template < class RealT >
THOR_INLINE RealT Sign(const RealT& f)
{
	if(f >= 0.0f) 
		return 1.0f;
	else 
		return -1.0f;	
}

template <class VecT>
THOR_INLINE void SignvOp(const VecT& arg , VecT& res)
{
	for( unsigned int i = 0; i < arg.Size(); ++i )
	{
		if( arg(i) >= 0.0f ) 
			res(i) = 1.0f;
		else 
			res(i) = -1.0f;
	}
}

template < class RealT >
THOR_INLINE RealT Abs(const RealT& f)
{
	return abs(f);
}

template <class VecT>
THOR_INLINE void AbsvOp(const VecT& arg , VecT& res)
{
	for( unsigned int i = 0; i < arg.Size(); ++i )
		res(i) = abs( arg(i) );
}

template < class RealT >
THOR_INLINE RealT Floor(const RealT& f)
{
	return floor(f);
}

template <class VecT>
THOR_INLINE void FloorvOp(const VecT& arg , VecT& res)
{
	for( unsigned int i = 0; i < arg.Size(); ++i )
		res(i) = floor( arg(i) );
}

template < class RealT >
THOR_INLINE RealT Ceil(const RealT& f)
{
	return ceil(f);
}

template <class VecT>
THOR_INLINE void CeilvOp(const VecT& arg , VecT& res)
{
	for( unsigned int i = 0; i < arg.Size(); ++i )
		res(i) = ceil( arg(i) );
}

template < class RealT >
THOR_INLINE RealT Clamp(const RealT& f , const RealT& min, const RealT& max)
{
	f= (f >= max) ? max : f;
	f= (f >= min) ? f : min;
	return f;
}

template <class VecT,class RealT>
THOR_INLINE void ClampvOp(const VecT& arg , const RealT& min, const RealT& max, VecT& res)
{
	for( unsigned int i = 0; i<arg.Size(); ++i )
	{
		res(i) = (arg(0) >= max) ? max : arg(0);
		res(i) = (arg(0) >= min) ? arg(0) : min;
	}
}

template < class RealT, class PowT >
THOR_INLINE RealT Pow(const RealT& f, const PowT& p)
{
	return pow(f, p);
}

template <class VecT,class PowVecT>
THOR_INLINE void PowvOp(const VecT& arg , const PowVecT& p, VecT& res)
{
	for( unsigned int i = 0; i < arg.Size(); ++i )
		res(i) = pow( arg(i), p(i) );	
}

template < class RealT >
THOR_INLINE RealT Exp(const RealT& f)
{
	return exp(f);
}

template <class VecT>
THOR_INLINE void ExpvOp(const VecT& arg , VecT& res)
{
	for( unsigned int i = 0; i < arg.Size(); ++i )
		res(i) = exp( arg(i) );
}

template < class RealT >
THOR_INLINE RealT Ln(const RealT& x)
{
	return log(x);
}

template <class VecT>
THOR_INLINE void LnvOp(const VecT& arg , VecT& res)
{
	for( unsigned int i = 0; i < arg.Size(); ++i )
		res(i) = log( arg(i) );
}

template < class RealT >
THOR_INLINE void MantissaExp( const RealT& f, RealT &m, RealT &e, RealT &p )
{
}
#pragma endregion

#ifdef USE_SSE_MATH

#include <Thor/ThorMath/MathSSE.h>

#endif

};//Math
};//Thor