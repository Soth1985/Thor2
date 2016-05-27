#pragma once

#include <smmintrin.h>
#include <Thor/Core/Common.h>
#include <Thor/Math/Constants.h>
#include <Thor/Math/FixedVector.h>


namespace Thor{ namespace Math{

THOR_INLINE __m128  Blend4(__m128 mask, __m128  arg_true, __m128  arg_false)
{
	//return _mm_or_ps(_mm_and_ps(mask, arg_true), _mm_andnot_ps(mask, arg_false));
	return _mm_blendv_ps(arg_false, arg_true, mask);
}

THOR_INLINE float Sqrt(float f)
{
	__m128 xmm0 = _mm_load_ss(&f);
	xmm0 = _mm_sqrt_ss(xmm0);
	_mm_store_ss(&f, xmm0);
	return f;
}

THOR_INLINE void SqrtvOp(const ThSseVec4& arg, ThSseVec4& res)
{
	__m128 xmm0 = _mm_load_ps(arg.Begin());
	xmm0 = _mm_sqrt_ps(xmm0);
	_mm_store_ps(res.Begin(), xmm0);
}

THOR_INLINE float InvSqrt(float f)
{
	__m128 xmm0 = _mm_load_ss(&f);
	xmm0 = _mm_rsqrt_ss(xmm0);
	_mm_store_ss(&f, xmm0);
	return f;
}

THOR_INLINE void InvSqrtvOp(const ThSseVec4& arg, ThSseVec4& res)
{
	__m128 xmm0 = _mm_load_ps(arg.Begin());
	xmm0 = _mm_rsqrt_ps(xmm0);
	_mm_store_ps(res.Begin(), xmm0);
}

THOR_INLINE float Sin(float f)
{
	using namespace SSE;
	__m128 xmm0 = _mm_load_ss(&f);
	__m128 xmm1 = _mm_mul_ss(xmm0, Simd_1div2Pi);			//xmm1=a/(2pi)
	xmm1 = _mm_sub_ss(xmm1, Simd_half);				// begin cutoff fractional part  xmm1=floor(xmm1)
	int cvt = _mm_cvt_ss2si(xmm1);					
	xmm1 = _mm_cvt_si2ss(xmm1, cvt);				// end cutoff fractional part
	xmm1 = _mm_mul_ss(xmm1, Simd_2Pi);
	xmm0 = _mm_sub_ss(xmm0, xmm1);					//xmm0 now is between 0 and -+2pi

	xmm1 = _mm_sub_ss(Simd_Pi, xmm0);					// xmm1 = PI - a
	xmm0 = _mm_and_ps(xmm1, Simd_signBitMask);		// xmm0 = signbit( PI - a )
	__m128 xmm2 = _mm_xor_ps(xmm1, xmm0);					// xmm2 = fabs( PI - a )
	xmm2 = _mm_cmpnlt_ss(xmm2, Simd_HalfPi);		// xmm2 = ( fabs( PI - a ) >= HALF_PI ) ? 0xFFFFFFFF : 0x00000000
	__m128 xmm3 = _mm_xor_ps(Simd_Pi, xmm0);					// xmm3 = PI xor signbit( PI - a )
	xmm3 = _mm_and_ps(xmm3, xmm2);					// xmm3 = ( fabs( PI - a ) >= HALF_PI ) ? ( PI xor signbit( PI - a ) ) : 0.0f
	xmm2 = _mm_and_ps(xmm2, Simd_signBitMask);		// xmm2 = ( fabs( PI - a ) >= HALF_PI ) ? Simd_signBitMask : 0.0f
	xmm1 = _mm_xor_ps(xmm1, xmm2);
	xmm1 = _mm_add_ps(xmm1, xmm3);

	xmm0 = _mm_mul_ss(xmm1, xmm1);					//xmm0=x^2
	xmm2 = _mm_mul_ss(Simd_SinC4, xmm0);					//xmm2=c4*x^2
	xmm2 = _mm_add_ss(xmm2, Simd_SinC3);			//xmm2=c4*x^2+c3
	xmm2 = _mm_mul_ss(xmm2, xmm0);					//xmm2=x^2*(c4*x^2+c3)
	xmm2 = _mm_add_ss(xmm2, Simd_SinC2);			//xmm2=c2+x^2*(c4*x^2+c3)
	xmm2 = _mm_mul_ss(xmm2, xmm0);					//xmm2=x^2*(c2+x^2*(c4*x^2+c3))
	xmm2 = _mm_add_ss(xmm2, Simd_SinC1);			//xmm2=c1+x^2*(c2+x^2*(c4*x^2+c3))
	xmm2 = _mm_mul_ss(xmm2, xmm0);					//xmm2=x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3)))
	xmm2 = _mm_add_ss(xmm2, Simd_SinC0);			//xmm2=c0+x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3)))
	xmm2 = _mm_mul_ss(xmm2, xmm0);					//xmm2=x^2*(c0+x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3))))
	xmm2 = _mm_add_ss(xmm2, Simd_one);				//xmm2=1.0+x^2*(c0+x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3))))
	xmm2 = _mm_mul_ss(xmm2, xmm1);					//xmm2=x*(1.0+x^2*(c0+x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3)))))

	_mm_store_ss(&f, xmm2);
	return f;
}

THOR_INLINE void SinvOp(const ThSseVec4& arg, ThSseVec4& res)
{
	using namespace SSE;
	__m128 xmm0 = _mm_load_ps(arg.Begin());
	__m128 xmm1 = _mm_mul_ps(xmm0, Simd_1div2Pi);			//xmm1=a/(2pi)
	xmm1 = _mm_sub_ps(xmm1, Simd_half);				// begin cutoff fractional part  xmm1=floor(xmm1)
	__m128i cvt = _mm_cvtps_epi32(xmm1);
	xmm1 = _mm_cvtepi32_ps(cvt);				// end cutoff fractional part
	xmm1 = _mm_mul_ps(xmm1, Simd_2Pi);
	xmm0 = _mm_sub_ps(xmm0, xmm1);					//xmm0 now is between 0 and -+2pi

	xmm1 = _mm_sub_ps(Simd_Pi, xmm0);					// xmm1 = PI - a
	xmm0 = _mm_and_ps(xmm1, Simd_signBitMask);		// xmm0 = signbit( PI - a )
	__m128 xmm2 = _mm_xor_ps(xmm1, xmm0);					// xmm2 = fabs( PI - a )
	xmm2 = _mm_cmpnlt_ps(xmm2, Simd_HalfPi);		// xmm2 = ( fabs( PI - a ) >= HALF_PI ) ? 0xFFFFFFFF : 0x00000000
	__m128 xmm3 = _mm_xor_ps(Simd_Pi, xmm0);					// xmm3 = PI xor signbit( PI - a )
	xmm3 = _mm_and_ps(xmm3, xmm2);					// xmm3 = ( fabs( PI - a ) >= HALF_PI ) ? ( PI xor signbit( PI - a ) ) : 0.0f
	xmm2 = _mm_and_ps(xmm2, Simd_signBitMask);		// xmm2 = ( fabs( PI - a ) >= HALF_PI ) ? Simd_signBitMask : 0.0f
	xmm1 = _mm_xor_ps(xmm1, xmm2);
	xmm1 = _mm_add_ps(xmm1, xmm3);

	xmm0 = _mm_mul_ps(xmm1, xmm1);					//xmm0=x^2
	xmm2 = _mm_mul_ps(Simd_SinC4, xmm0);					//xmm2=c4*x^2
	xmm2 = _mm_add_ps(xmm2, Simd_SinC3);			//xmm2=c4*x^2+c3
	xmm2 = _mm_mul_ps(xmm2, xmm0);					//xmm2=x^2*(c4*x^2+c3)
	xmm2 = _mm_add_ps(xmm2, Simd_SinC2);			//xmm2=c2+x^2*(c4*x^2+c3)
	xmm2 = _mm_mul_ps(xmm2, xmm0);					//xmm2=x^2*(c2+x^2*(c4*x^2+c3))
	xmm2 = _mm_add_ps(xmm2, Simd_SinC1);			//xmm2=c1+x^2*(c2+x^2*(c4*x^2+c3))
	xmm2 = _mm_mul_ps(xmm2, xmm0);					//xmm2=x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3)))
	xmm2 = _mm_add_ps(xmm2, Simd_SinC0);			//xmm2=c0+x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3)))
	xmm2 = _mm_mul_ps(xmm2, xmm0);					//xmm2=x^2*(c0+x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3))))
	xmm2 = _mm_add_ps(xmm2, Simd_one);				//xmm2=1.0+x^2*(c0+x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3))))
	xmm2 = _mm_mul_ps(xmm2, xmm1);					//xmm2=x*(1.0+x^2*(c0+x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3)))))

	_mm_store_ps(res.Begin(), xmm2);
}

THOR_INLINE float Cos(float f)
{
	using namespace SSE;
	__m128 xmm0 = _mm_load_ss(&f);
	__m128 xmm1 = _mm_mul_ss(xmm0, Simd_1div2Pi);			//xmm1=a/(2pi)
	xmm1 = _mm_sub_ss(xmm1, Simd_half);				// begin cutoff fractional part  xmm1=floor(xmm1)
	int cvt = _mm_cvt_ss2si(xmm1);
	xmm1 = _mm_cvt_si2ss(xmm1, cvt);				// end cutoff fractional part
	xmm1 = _mm_mul_ss(xmm1, Simd_2Pi);
	xmm0 = _mm_sub_ss(xmm0, xmm1);					//xmm0 now is between 0 and -+2pi

	xmm1 = _mm_sub_ss(Simd_Pi, xmm0);					// xmm1 = PI - a
	xmm0 = _mm_and_ps(xmm1, Simd_signBitMask);		// xmm0 = signbit( PI - a )
	__m128 xmm2 = _mm_xor_ps(xmm1, xmm0);					// xmm2 = fabs( PI - a )
	xmm2 = _mm_cmpnlt_ss(xmm2, Simd_HalfPi);		// xmm2 = ( fabs( PI - a ) >= HALF_PI ) ? 0xFFFFFFFF : 0x00000000
	__m128 xmm3 = _mm_xor_ps(Simd_Pi, xmm0);					// xmm3 = PI xor signbit( PI - a )
	xmm3 = _mm_and_ps(xmm3, xmm2);					// xmm3 = ( fabs( PI - a ) >= HALF_PI ) ? ( PI xor signbit( PI - a ) ) : 0.0f
	xmm2 = _mm_and_ps(xmm2, Simd_signBitMask);		// xmm2 = ( fabs( PI - a ) >= HALF_PI ) ? Simd_signBitMask : 0.0f
	xmm1 = _mm_xor_ps(xmm1, xmm2);
	xmm1 = _mm_add_ps(xmm1, xmm3);

	xmm0 = _mm_mul_ss(xmm1, xmm1);			//xmm0=x^2
	xmm3 = _mm_mul_ss(Simd_CosC4, xmm0);	//xmm3=c4*x^2
	xmm3 = _mm_add_ss(xmm3, Simd_CosC3);	//xmm3=c4*x^2+c3
	xmm3 = _mm_mul_ss(xmm3, xmm0);				//xmm3=x^2*(c4*x^2+c3)
	xmm3 = _mm_add_ss(xmm3, Simd_CosC2);		//xmm3=c2+x^2*(c4*x^2+c3)
	xmm3 = _mm_mul_ss(xmm3, xmm0);				//xmm3=x^2*(c2+x^2*(c4*x^2+c3))
	xmm3 = _mm_add_ss(xmm3, Simd_CosC1);		//xmm3=c1+x^2*(c2+x^2*(c4*x^2+c3))
	xmm3 = _mm_mul_ss(xmm3, xmm0);				//xmm3=x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3)))
	xmm3 = _mm_add_ss(xmm3, Simd_CosC0);		//xmm3=c0+x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3)))
	xmm3 = _mm_mul_ss(xmm3, xmm0);				//xmm3=x^2*(c0+x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3))))
	xmm3 = _mm_add_ss(xmm3, Simd_one);			//xmm3=1.0+x^2*(c0+x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3))))
	xmm2 = _mm_xor_ps(xmm2, Simd_signBitMask);
	xmm3 = _mm_xor_ps(xmm3, xmm2);

	_mm_store_ss(&f, xmm3);
	return f;
}

THOR_INLINE void CosvOp(const ThSseVec4& arg, ThSseVec4& res)
{
	using namespace SSE;
	__m128 xmm0 = _mm_load_ps(arg.Begin());
	__m128 xmm1 = _mm_mul_ps(xmm0, Simd_1div2Pi);			//xmm1=a/(2pi)
	xmm1 = _mm_sub_ps(xmm1, Simd_half);				// begin cutoff fractional part  xmm1=floor(xmm1)
	__m128i cvt = _mm_cvtps_epi32(xmm1);
	xmm1 = _mm_cvtepi32_ps(cvt);				// end cutoff fractional part
	xmm1 = _mm_mul_ps(xmm1, Simd_2Pi);
	xmm0 = _mm_sub_ps(xmm0, xmm1);					//xmm0 now is between 0 and -+2pi

	xmm1 = _mm_sub_ps(Simd_Pi, xmm0);					// xmm1 = PI - a
	xmm0 = _mm_and_ps(xmm1, Simd_signBitMask);		// xmm0 = signbit( PI - a )
	__m128 xmm2 = _mm_xor_ps(xmm1, xmm0);					// xmm2 = fabs( PI - a )
	xmm2 = _mm_cmpnlt_ps(xmm2, Simd_HalfPi);		// xmm2 = ( fabs( PI - a ) >= HALF_PI ) ? 0xFFFFFFFF : 0x00000000
	__m128 xmm3 = _mm_xor_ps(Simd_Pi, xmm0);					// xmm3 = PI xor signbit( PI - a )
	xmm3 = _mm_and_ps(xmm3, xmm2);					// xmm3 = ( fabs( PI - a ) >= HALF_PI ) ? ( PI xor signbit( PI - a ) ) : 0.0f
	xmm2 = _mm_and_ps(xmm2, Simd_signBitMask);		// xmm2 = ( fabs( PI - a ) >= HALF_PI ) ? Simd_signBitMask : 0.0f
	xmm1 = _mm_xor_ps(xmm1, xmm2);
	xmm1 = _mm_add_ps(xmm1, xmm3);

	xmm0 = _mm_mul_ps(xmm1, xmm1);			//xmm0=x^2
	xmm3 = _mm_mul_ps(Simd_CosC4, xmm0);	//xmm3=c4*x^2
	xmm3 = _mm_add_ps(xmm3, Simd_CosC3);	//xmm3=c4*x^2+c3
	xmm3 = _mm_mul_ps(xmm3, xmm0);				//xmm3=x^2*(c4*x^2+c3)
	xmm3 = _mm_add_ps(xmm3, Simd_CosC2);		//xmm3=c2+x^2*(c4*x^2+c3)
	xmm3 = _mm_mul_ps(xmm3, xmm0);				//xmm3=x^2*(c2+x^2*(c4*x^2+c3))
	xmm3 = _mm_add_ps(xmm3, Simd_CosC1);		//xmm3=c1+x^2*(c2+x^2*(c4*x^2+c3))
	xmm3 = _mm_mul_ps(xmm3, xmm0);				//xmm3=x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3)))
	xmm3 = _mm_add_ps(xmm3, Simd_CosC0);		//xmm3=c0+x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3)))
	xmm3 = _mm_mul_ps(xmm3, xmm0);				//xmm3=x^2*(c0+x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3))))
	xmm3 = _mm_add_ps(xmm3, Simd_one);			//xmm3=1.0+x^2*(c0+x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3))))
	xmm2 = _mm_xor_ps(xmm2, Simd_signBitMask);
	xmm3 = _mm_xor_ps(xmm3, xmm2);

	_mm_store_ps(res.Begin(), xmm3);
}

THOR_INLINE void SinCos(float f, float &s, float &c)
{
	using namespace SSE;
	__m128 xmm0 = _mm_load_ss(&f);
	__m128 xmm1 = _mm_mul_ss(xmm0, Simd_1div2Pi);			//xmm1=a/(2pi)
	xmm1 = _mm_sub_ss(xmm1, Simd_half);				// begin cutoff fractional part  xmm1=floor(xmm1)
	int cvt = _mm_cvt_ss2si(xmm1);
	xmm1 = _mm_cvt_si2ss(xmm1, cvt);				// end cutoff fractional part
	xmm1 = _mm_mul_ss(xmm1, Simd_2Pi);
	xmm0 = _mm_sub_ss(xmm0, xmm1);					//xmm0 now is between 0 and -+2pi

	xmm1 = _mm_sub_ss(Simd_Pi, xmm0);					// xmm1 = PI - a
	xmm0 = _mm_and_ps(xmm1, Simd_signBitMask);		// xmm0 = signbit( PI - a )
	__m128 xmm2 = _mm_xor_ps(xmm1, xmm0);					// xmm2 = fabs( PI - a )
	xmm2 = _mm_cmpnlt_ss(xmm2, Simd_HalfPi);		// xmm2 = ( fabs( PI - a ) >= HALF_PI ) ? 0xFFFFFFFF : 0x00000000
	__m128 xmm3 = _mm_xor_ps(Simd_Pi, xmm0);					// xmm3 = PI xor signbit( PI - a )
	xmm3 = _mm_and_ps(xmm3, xmm2);					// xmm3 = ( fabs( PI - a ) >= HALF_PI ) ? ( PI xor signbit( PI - a ) ) : 0.0f
	xmm2 = _mm_and_ps(xmm2, Simd_signBitMask);		// xmm2 = ( fabs( PI - a ) >= HALF_PI ) ? Simd_signBitMask : 0.0f
	xmm1 = _mm_xor_ps(xmm1, xmm2);
	xmm1 = _mm_add_ps(xmm1, xmm3);

	xmm0 = _mm_mul_ss(xmm1, xmm1);
	xmm3 = _mm_mul_ss(Simd_CosC4, xmm0);
	__m128 xmm4 = _mm_mul_ss(Simd_SinC4, xmm0);
	xmm3 = _mm_add_ss(xmm3, Simd_CosC3);
	xmm4 = _mm_add_ss(xmm4, Simd_SinC3);
	xmm3 = _mm_mul_ss(xmm3, xmm0);
	xmm4 = _mm_mul_ss(xmm4, xmm0);
	xmm3 = _mm_add_ss(xmm3, Simd_CosC2);
	xmm4 = _mm_add_ss(xmm4, Simd_SinC2);
	xmm3 = _mm_mul_ss(xmm3, xmm0);
	xmm4 = _mm_mul_ss(xmm4, xmm0);
	xmm3 = _mm_add_ss(xmm3, Simd_CosC1);
	xmm4 = _mm_add_ss(xmm4, Simd_SinC1);
	xmm3 = _mm_mul_ss(xmm3, xmm0);
	xmm4 = _mm_mul_ss(xmm4, xmm0);
	xmm3 = _mm_add_ss(xmm3, Simd_CosC0);
	xmm4 = _mm_add_ss(xmm4, Simd_SinC0);
	xmm3 = _mm_mul_ss(xmm3, xmm0);
	xmm4 = _mm_mul_ss(xmm4, xmm0);
	xmm3 = _mm_add_ss(xmm3, Simd_one);
	xmm4 = _mm_add_ss(xmm4, Simd_one);
	xmm4 = _mm_mul_ss(xmm4, xmm1);
	xmm2 = _mm_xor_ps(xmm2, Simd_signBitMask);
	xmm3 = _mm_xor_ps(xmm3, xmm2);

	_mm_store_ss(&s, xmm4);
	_mm_store_ss(&c, xmm3);
}

THOR_INLINE void SinCosvOp(const ThSseVec4& arg, ThSseVec4& s, ThSseVec4& c)
{
	using namespace SSE;
	__m128 xmm0 = _mm_load_ps(arg.Begin());
	__m128 xmm1 = _mm_mul_ps(xmm0, Simd_1div2Pi);			//xmm1=a/(2pi)
	xmm1 = _mm_sub_ps(xmm1, Simd_half);				// begin cutoff fractional part  xmm1=floor(xmm1)
	__m128i cvt = _mm_cvtps_epi32(xmm1);
	xmm1 = _mm_cvtepi32_ps(cvt);				// end cutoff fractional part
	xmm1 = _mm_mul_ps(xmm1, Simd_2Pi);
	xmm0 = _mm_sub_ps(xmm0, xmm1);					//xmm0 now is between 0 and -+2pi

	xmm1 = _mm_sub_ps(Simd_Pi, xmm0);					// xmm1 = PI - a
	xmm0 = _mm_and_ps(xmm1, Simd_signBitMask);		// xmm0 = signbit( PI - a )
	__m128 xmm2 = _mm_xor_ps(xmm1, xmm0);					// xmm2 = fabs( PI - a )
	xmm2 = _mm_cmpnlt_ps(xmm2, Simd_HalfPi);		// xmm2 = ( fabs( PI - a ) >= HALF_PI ) ? 0xFFFFFFFF : 0x00000000
	__m128 xmm3 = _mm_xor_ps(Simd_Pi, xmm0);					// xmm3 = PI xor signbit( PI - a )
	xmm3 = _mm_and_ps(xmm3, xmm2);					// xmm3 = ( fabs( PI - a ) >= HALF_PI ) ? ( PI xor signbit( PI - a ) ) : 0.0f
	xmm2 = _mm_and_ps(xmm2, Simd_signBitMask);		// xmm2 = ( fabs( PI - a ) >= HALF_PI ) ? Simd_signBitMask : 0.0f
	xmm1 = _mm_xor_ps(xmm1, xmm2);
	xmm1 = _mm_add_ps(xmm1, xmm3);

	xmm0 = _mm_mul_ps(xmm1, xmm1);
	xmm3 = _mm_mul_ps(Simd_CosC4, xmm0);
	__m128 xmm4 = _mm_mul_ps(Simd_SinC4, xmm0);
	xmm3 = _mm_add_ps(xmm3, Simd_CosC3);
	xmm4 = _mm_add_ps(xmm4, Simd_SinC3);
	xmm3 = _mm_mul_ps(xmm3, xmm0);
	xmm4 = _mm_mul_ps(xmm4, xmm0);
	xmm3 = _mm_add_ps(xmm3, Simd_CosC2);
	xmm4 = _mm_add_ps(xmm4, Simd_SinC2);
	xmm3 = _mm_mul_ps(xmm3, xmm0);
	xmm4 = _mm_mul_ps(xmm4, xmm0);
	xmm3 = _mm_add_ps(xmm3, Simd_CosC1);
	xmm4 = _mm_add_ps(xmm4, Simd_SinC1);
	xmm3 = _mm_mul_ps(xmm3, xmm0);
	xmm4 = _mm_mul_ps(xmm4, xmm0);
	xmm3 = _mm_add_ps(xmm3, Simd_CosC0);
	xmm4 = _mm_add_ps(xmm4, Simd_SinC0);
	xmm3 = _mm_mul_ps(xmm3, xmm0);
	xmm4 = _mm_mul_ps(xmm4, xmm0);
	xmm3 = _mm_add_ps(xmm3, Simd_one);
	xmm4 = _mm_add_ps(xmm4, Simd_one);
	xmm4 = _mm_mul_ps(xmm4, xmm1);
	xmm2 = _mm_xor_ps(xmm2, Simd_signBitMask);
	xmm3 = _mm_xor_ps(xmm3, xmm2);

	_mm_store_ps(s.Begin(), xmm4);
	_mm_store_ps(c.Begin(), xmm3);
}

THOR_INLINE float Atan(float f)
{
	using namespace SSE;
	//atan(x)=x/(1+0.28*x^2)   if   abs(x)<=1
	//atan(x)=pi/2-x/(0.28+x^2)   if   abs(x)>1  x>0
	//atan(x)=-pi/2+x/(0.28+x^2)   if   abs(x)>1  x<0
	//atan(x)=xmm6|(xmm4+x/ ( xmm2&(1+0.28*x^2) + xmm3&(0.28+x^2)))
	__m128 xmm0 = _mm_load_ss(&f);
	__m128 xmm1 = _mm_and_ps(xmm0, Simd_absMask);		//xmm1=abs(x)		
	__m128 xmm6 = _mm_and_ps(xmm0, Simd_signBitMask);	//xmm6=signbit
	xmm0 = xmm1;
	xmm1 = _mm_cmple_ps(xmm1, Simd_one);			//abs(x)<=1 ? 0xffffffff:0.0 
	__m128 xmm2 = xmm1;
	__m128 xmm3 = _mm_xor_ps(xmm1, Simd_not);
	__m128 xmm4 = _mm_and_ps(Simd_HalfPi, xmm3);

	xmm1 = _mm_mul_ss(xmm0, xmm0);
	__m128 xmm5 = _mm_mul_ss(xmm1, Simd_Atan);
	xmm5 = _mm_add_ss(xmm5, Simd_one);
	xmm5 = _mm_and_ps(xmm5, xmm2);
	xmm1 = _mm_add_ss(xmm1, Simd_Atan);
	xmm1 = _mm_and_ps(xmm1, xmm3);
	xmm5 = _mm_sub_ss(xmm5, xmm1);

	xmm2 = _mm_rcp_ss(xmm5);
	xmm2 = _mm_mul_ss(xmm2, xmm0);
	xmm2 = _mm_add_ss(xmm2, xmm4);
	xmm2 = _mm_or_ps(xmm2, xmm6);
	_mm_store_ss(&f, xmm2);

	return f;
}

THOR_INLINE void AtanvOp(const ThSseVec4& arg, ThSseVec4& res)
{
	using namespace SSE;
	//atan(x)=x/(1+0.28*x^2)   if   abs(x)<=1
	//atan(x)=pi/2-x/(0.28+x^2)   if   abs(x)>1  x>0
	//atan(x)=-pi/2+x/(0.28+x^2)   if   abs(x)>1  x<0
	//atan(x)=xmm6|(xmm4+x/ ( xmm2&(1+0.28*x^2) + xmm3&(0.28+x^2)))
	__m128 xmm0 = _mm_load_ps(arg.Begin());
	__m128 xmm1 = _mm_and_ps(xmm0, Simd_absMask);		//xmm1=abs(x)		
	__m128 xmm6 = _mm_and_ps(xmm0, Simd_signBitMask);	//xmm6=signbit
	xmm0 = xmm1;
	xmm1 = _mm_cmple_ps(xmm1, Simd_one);			//abs(x)<=1 ? 0xffffffff:0.0 
	__m128 xmm2 = xmm1;
	__m128 xmm3 = _mm_xor_ps(xmm1, Simd_not);
	__m128 xmm4 = _mm_and_ps(Simd_HalfPi, xmm3);

	xmm1 = _mm_mul_ps(xmm0, xmm0);
	__m128 xmm5 = _mm_mul_ps(xmm1, Simd_Atan);
	xmm5 = _mm_add_ps(xmm5, Simd_one);
	xmm5 = _mm_and_ps(xmm5, xmm2);
	xmm1 = _mm_add_ps(xmm1, Simd_Atan);
	xmm1 = _mm_and_ps(xmm1, xmm3);
	xmm5 = _mm_sub_ps(xmm5, xmm1);

	xmm2 = _mm_rcp_ps(xmm5);
	xmm2 = _mm_mul_ps(xmm2, xmm0);
	xmm2 = _mm_add_ps(xmm2, xmm4);
	xmm2 = _mm_or_ps(xmm2, xmm6);
	_mm_store_ps(res.Begin(), xmm2);
}

THOR_INLINE float Atan2(float y, float x)
{
	//x > 1, compute arctan(x) = pi / 2 - arctan(1 / x)
	//arctan(y / x) x > 0; arctan(y / x) + pi y >=0 x < 0; arctan(y / x) - pi y < 0 x < 0; pi/2 y > 0 x = 0; -pi/2 y < 0 x = 0; 0 x,y =0
	using namespace SSE;
	__m128 xmm0 = _mm_load_ss(&x);
	__m128 xmm1 = _mm_load_ss(&y);
	__m128 x_neg = _mm_cmplt_ss(xmm0, Simd_zero);
	__m128 x_zero = _mm_cmpeq_ss(xmm0, Simd_zero);
	__m128 y_neg = _mm_cmplt_ss(xmm1, Simd_zero);
	__m128 y_zero = _mm_cmpeq_ss(xmm1, Simd_zero);
	__m128 xmm2 = _mm_rcp_ss(xmm0);
	xmm0 = _mm_mul_ss(xmm1, xmm2);
	__m128 is_neg = _mm_cmplt_ss(xmm0, Simd_zero);	
	xmm0 = _mm_and_ps(xmm0, Simd_absMask);
	xmm1 = _mm_rcp_ss(xmm0);
	__m128 xmm3 = _mm_cmpgt_ss(xmm0, Simd_one);
	xmm0 = Blend4(xmm3, xmm1, xmm0);

	xmm1 = _mm_mul_ss(xmm0, xmm0);				// xmm1 = s
	xmm2 = _mm_mul_ss(Simd_atan_c0, xmm1);
	xmm2 = _mm_add_ss(xmm2, Simd_atan_c1);
	xmm2 = _mm_mul_ss(xmm2, xmm1);
	xmm2 = _mm_add_ss(xmm2, Simd_atan_c2);
	xmm2 = _mm_mul_ss(xmm2, xmm1);
	xmm2 = _mm_add_ss(xmm2, Simd_atan_c3);
	xmm2 = _mm_mul_ss(xmm2, xmm1);
	xmm2 = _mm_add_ss(xmm2, Simd_atan_c4);
	xmm2 = _mm_mul_ss(xmm2, xmm1);
	xmm2 = _mm_add_ss(xmm2, Simd_atan_c5);
	xmm2 = _mm_mul_ss(xmm2, xmm1);
	xmm2 = _mm_add_ss(xmm2, Simd_atan_c6);
	xmm2 = _mm_mul_ss(xmm2, xmm1);
	xmm2 = _mm_add_ss(xmm2, Simd_atan_c7);
	xmm2 = _mm_mul_ss(xmm2, xmm1);
	xmm2 = _mm_add_ss(xmm2, Simd_one);
	xmm2 = _mm_mul_ss(xmm2, xmm0);

	__m128 sign_conv = Blend4(is_neg, Simd_MinusOne, Simd_one);
	__m128 signs_fixed = _mm_mul_ss(sign_conv, xmm2);

	// correct the output range for all inverted input by
	// either subtracting from PI/2 or -PI/2, depending on the
	// sign of signs_fixed (which matches the neg_x mask)
	__m128 base = Blend4(is_neg, Simd_MinHalfPi, Simd_HalfPi);
	xmm1 = _mm_sub_ss(base, signs_fixed);
	__m128 res_14 = Blend4(xmm3, xmm1, signs_fixed);
	//calc atan2 from atan
	__m128 res_2 = _mm_add_ss(res_14, Simd_Pi);
	__m128 res_3 = _mm_add_ss(res_14, Simd_MinPi);
	__m128 res = Blend4(y_neg, res_3, res_2);
	res = Blend4(x_neg, res, res_14);

	__m128 res_x0 = Blend4(y_neg, Simd_MinHalfPi, Simd_HalfPi);
	res_x0 = Blend4(y_zero, Simd_zero, res_x0);
	res = Blend4(x_zero, res_x0, res);
	
	_mm_store_ss(&y, res);
	return y;
}

THOR_INLINE void Atan2vOp(const ThSseVec4& y, const ThSseVec4& x, ThSseVec4& result)
{
	//x > 1, compute arctan(x) = pi / 2 - arctan(1 / x)
	//arctan(y / x) x > 0; arctan(y / x) + pi y >=0 x < 0; arctan(y / x) - pi y < 0 x < 0; pi/2 y > 0 x = 0; -pi/2 y < 0 x = 0; 0 x,y =0
	using namespace SSE;
	__m128 xmm0 = _mm_load_ps(x.Begin());
	__m128 xmm1 = _mm_load_ps(y.Begin());
	__m128 x_neg = _mm_cmplt_ps(xmm0, Simd_zero);
	__m128 x_zero = _mm_cmpeq_ps(xmm0, Simd_zero);
	__m128 y_neg = _mm_cmplt_ps(xmm1, Simd_zero);
	__m128 y_zero = _mm_cmpeq_ps(xmm1, Simd_zero);
	__m128 xmm2 = _mm_rcp_ps(xmm0);
	xmm0 = _mm_mul_ps(xmm1, xmm2);
	__m128 is_neg = _mm_cmplt_ps(xmm0, Simd_zero);
	xmm0 = _mm_and_ps(xmm0, Simd_absMask);
	xmm1 = _mm_rcp_ps(xmm0);
	__m128 xmm3 = _mm_cmpgt_ps(xmm0, Simd_one);
	xmm0 = Blend4(xmm3, xmm1, xmm0);

	xmm1 = _mm_mul_ps(xmm0, xmm0);				// xmm1 = s
	xmm2 = _mm_mul_ps(Simd_atan_c0, xmm1);
	xmm2 = _mm_add_ps(xmm2, Simd_atan_c1);
	xmm2 = _mm_mul_ps(xmm2, xmm1);
	xmm2 = _mm_add_ps(xmm2, Simd_atan_c2);
	xmm2 = _mm_mul_ps(xmm2, xmm1);
	xmm2 = _mm_add_ps(xmm2, Simd_atan_c3);
	xmm2 = _mm_mul_ps(xmm2, xmm1);
	xmm2 = _mm_add_ps(xmm2, Simd_atan_c4);
	xmm2 = _mm_mul_ps(xmm2, xmm1);
	xmm2 = _mm_add_ps(xmm2, Simd_atan_c5);
	xmm2 = _mm_mul_ps(xmm2, xmm1);
	xmm2 = _mm_add_ps(xmm2, Simd_atan_c6);
	xmm2 = _mm_mul_ps(xmm2, xmm1);
	xmm2 = _mm_add_ps(xmm2, Simd_atan_c7);
	xmm2 = _mm_mul_ps(xmm2, xmm1);
	xmm2 = _mm_add_ps(xmm2, Simd_one);
	xmm2 = _mm_mul_ps(xmm2, xmm0);

	__m128 sign_conv = Blend4(is_neg, Simd_MinusOne, Simd_one);
	__m128 signs_fixed = _mm_mul_ps(sign_conv, xmm2);

	// correct the output range for all inverted input by
	// either subtracting from PI/2 or -PI/2, depending on the
	// sign of signs_fixed
	__m128 base = Blend4(is_neg, Simd_MinHalfPi, Simd_HalfPi);
	xmm1 = _mm_sub_ps(base, signs_fixed);
	__m128 res_14 = Blend4(xmm3, xmm1, signs_fixed);
	//calc atan2 from atan
	__m128 res_2 = _mm_add_ps(res_14, Simd_Pi);
	__m128 res_3 = _mm_add_ps(res_14, Simd_MinPi);
	__m128 res = Blend4(y_neg, res_3, res_2);
	res = Blend4(x_neg, res, res_14);

	__m128 res_x0 = Blend4(y_neg, Simd_MinHalfPi, Simd_HalfPi);
	res_x0 = Blend4(y_zero, Simd_zero, res_x0);
	res = Blend4(x_zero, res_x0, res);

	_mm_store_ps(result.Begin(), res);
}

THOR_INLINE float Asin(float f)
{
	using namespace SSE;
	// if abs(x)>0.5  asin(x)=pi/2-2asin(sqrt( (1-x)/2 ) )
	// asin(x)=xmm5 | ( xmm4&( pi/2- 2*asin)+ xmm3&( asin )
	__m128 xmm0 = _mm_load_ss(&f);
	__m128 xmm1 = _mm_and_ps(xmm0, Simd_absMask);		//xmm1=abs(x)
	__m128 xmm5 = _mm_and_ps(xmm0, Simd_signBitMask);	//xmm5=signbit
	__m128 xmm2 = xmm1;
	__m128 xmm3 = _mm_sub_ss(Simd_one, xmm1);			//calc  xmm3=sqrt( (1-x)/2 )
	xmm3 = _mm_mul_ss(xmm3, Simd_half);
	xmm3 = _mm_sqrt_ss(xmm3);
	xmm1 = _mm_min_ss(xmm1, xmm3);			   //choose min
	xmm0 = xmm1;		   //set min as argument
	xmm2 = _mm_cmplt_ps(xmm2, Simd_half);			//is abs(x) > 0.5 
	xmm3 = xmm2;

	xmm2 = _mm_xor_ps(xmm2, Simd_not);
	__m128 xmm4 = xmm2;

	xmm1 = _mm_mul_ss(xmm1, xmm1);
	xmm2 = _mm_mul_ss(Simd_asin_c9, xmm1);
	xmm2 = _mm_add_ss(xmm2, Simd_asin_c8);
	xmm2 = _mm_mul_ss(xmm2, xmm1);
	xmm2 = _mm_add_ss(xmm2, Simd_asin_c7);
	xmm2 = _mm_mul_ss(xmm2, xmm1);
	xmm2 = _mm_add_ss(xmm2, Simd_asin_c6);
	xmm2 = _mm_mul_ss(xmm2, xmm1);
	xmm2 = _mm_add_ss(xmm2, Simd_asin_c5);
	xmm2 = _mm_mul_ss(xmm2, xmm1);
	xmm2 = _mm_add_ss(xmm2, Simd_asin_c4);
	xmm2 = _mm_mul_ss(xmm2, xmm1);
	xmm2 = _mm_add_ss(xmm2, Simd_asin_c3);
	xmm2 = _mm_mul_ss(xmm2, xmm1);
	xmm2 = _mm_add_ss(xmm2, Simd_asin_c2);
	xmm2 = _mm_mul_ss(xmm2, xmm1);
	xmm2 = _mm_add_ss(xmm2, Simd_asin_c1);
	xmm2 = _mm_mul_ss(xmm2, xmm1);
	xmm2 = _mm_add_ss(xmm2, Simd_asin_c0);
	xmm2 = _mm_mul_ss(xmm2, xmm1);
	xmm2 = _mm_add_ss(xmm2, Simd_one);
	xmm2 = _mm_mul_ss(xmm2, xmm0);

	xmm1 = _mm_mul_ss(Simd_MinusTwo, xmm2);
	xmm1 = _mm_add_ss(xmm1, Simd_HalfPi);
	xmm1 = _mm_and_ps(xmm1, xmm4);
	xmm2 = _mm_and_ps(xmm2, xmm3);
	xmm1 = _mm_add_ss(xmm1, xmm2);
	xmm1 = _mm_or_ps(xmm1, xmm5);				//asin(-x)=-asin(x)

	_mm_store_ss(&f, xmm1);
	return f;
}

THOR_INLINE void AsinvOp(const ThSseVec4& arg, ThSseVec4& res)
{
	using namespace SSE;
	// if abs(x)>0.5  asin(x)=pi/2-2asin(sqrt( (1-x)/2 ) )
	// asin(x)=xmm5 | ( xmm4&( pi/2- 2*asin)+ xmm3&( asin )
	__m128 xmm0 = _mm_load_ps(arg.Begin());
	__m128 xmm1 = _mm_and_ps(xmm0, Simd_absMask);		//xmm1=abs(x)
	__m128 xmm5 = _mm_and_ps(xmm0, Simd_signBitMask);	//xmm5=signbit
	__m128 xmm2 = xmm1;
	__m128 xmm3 = _mm_sub_ps(Simd_one, xmm1);			//calc  xmm3=sqrt( (1-x)/2 )
	xmm3 = _mm_mul_ps(xmm3, Simd_half);
	xmm3 = _mm_sqrt_ps(xmm3);
	xmm1 = _mm_min_ps(xmm1, xmm3);			   //choose min
	xmm0 = xmm1;		   //set min as argument
	xmm2 = _mm_cmplt_ps(xmm2, Simd_half);			//is abs(x) > 0.5 
	xmm3 = xmm2;

	xmm2 = _mm_xor_ps(xmm2, Simd_not);
	__m128 xmm4 = xmm2;

	xmm1 = _mm_mul_ps(xmm1, xmm1);
	xmm2 = _mm_mul_ps(Simd_asin_c9, xmm1);
	xmm2 = _mm_add_ps(xmm2, Simd_asin_c8);
	xmm2 = _mm_mul_ps(xmm2, xmm1);
	xmm2 = _mm_add_ps(xmm2, Simd_asin_c7);
	xmm2 = _mm_mul_ps(xmm2, xmm1);
	xmm2 = _mm_add_ps(xmm2, Simd_asin_c6);
	xmm2 = _mm_mul_ps(xmm2, xmm1);
	xmm2 = _mm_add_ps(xmm2, Simd_asin_c5);
	xmm2 = _mm_mul_ps(xmm2, xmm1);
	xmm2 = _mm_add_ps(xmm2, Simd_asin_c4);
	xmm2 = _mm_mul_ps(xmm2, xmm1);
	xmm2 = _mm_add_ps(xmm2, Simd_asin_c3);
	xmm2 = _mm_mul_ps(xmm2, xmm1);
	xmm2 = _mm_add_ps(xmm2, Simd_asin_c2);
	xmm2 = _mm_mul_ps(xmm2, xmm1);
	xmm2 = _mm_add_ps(xmm2, Simd_asin_c1);
	xmm2 = _mm_mul_ps(xmm2, xmm1);
	xmm2 = _mm_add_ps(xmm2, Simd_asin_c0);
	xmm2 = _mm_mul_ps(xmm2, xmm1);
	xmm2 = _mm_add_ps(xmm2, Simd_one);
	xmm2 = _mm_mul_ps(xmm2, xmm0);

	xmm1 = _mm_mul_ps(Simd_MinusTwo, xmm2);
	xmm1 = _mm_add_ps(xmm1, Simd_HalfPi);
	xmm1 = _mm_and_ps(xmm1, xmm4);
	xmm2 = _mm_and_ps(xmm2, xmm3);
	xmm1 = _mm_add_ps(xmm1, xmm2);
	xmm1 = _mm_or_ps(xmm1, xmm5);				//asin(-x)=-asin(x)

	_mm_store_ps(res.Begin(), xmm1);
}

THOR_INLINE float Acos(float f)
{
	using namespace SSE;
	
	__m128 xmm0 = _mm_load_ss(&f);
	__m128 xmm1 = _mm_and_ps(xmm0, Simd_absMask);		//xmm1=abs(x)
	__m128 xmm5 = _mm_and_ps(xmm0, Simd_signBitMask);	//xmm5=signbit
	__m128 xmm2 = xmm1;
	__m128 xmm3 = _mm_sub_ss(Simd_one, xmm1);			//calc  xmm3=sqrt( (1-x)/2 )
	xmm3 = _mm_mul_ss(xmm3, Simd_half);
	xmm3 = _mm_sqrt_ss(xmm3);
	xmm1 = _mm_min_ss(xmm1, xmm3);			   //choose min
	xmm0 = xmm1;		   //set min as argument
	xmm2 = _mm_cmplt_ps(xmm2, Simd_half);			//is abs(x) > 0.5 
	xmm3 = xmm2;

	xmm2 = _mm_xor_ps(xmm2, Simd_not);
	__m128 xmm4 = xmm2;

	xmm1 = _mm_mul_ss(xmm1, xmm1);
	xmm2 = _mm_mul_ss(Simd_asin_c9, xmm1);
	xmm2 = _mm_add_ss(xmm2, Simd_asin_c8);
	xmm2 = _mm_mul_ss(xmm2, xmm1);
	xmm2 = _mm_add_ss(xmm2, Simd_asin_c7);
	xmm2 = _mm_mul_ss(xmm2, xmm1);
	xmm2 = _mm_add_ss(xmm2, Simd_asin_c6);
	xmm2 = _mm_mul_ss(xmm2, xmm1);
	xmm2 = _mm_add_ss(xmm2, Simd_asin_c5);
	xmm2 = _mm_mul_ss(xmm2, xmm1);
	xmm2 = _mm_add_ss(xmm2, Simd_asin_c4);
	xmm2 = _mm_mul_ss(xmm2, xmm1);
	xmm2 = _mm_add_ss(xmm2, Simd_asin_c3);
	xmm2 = _mm_mul_ss(xmm2, xmm1);
	xmm2 = _mm_add_ss(xmm2, Simd_asin_c2);
	xmm2 = _mm_mul_ss(xmm2, xmm1);
	xmm2 = _mm_add_ss(xmm2, Simd_asin_c1);
	xmm2 = _mm_mul_ss(xmm2, xmm1);
	xmm2 = _mm_add_ss(xmm2, Simd_asin_c0);
	xmm2 = _mm_mul_ss(xmm2, xmm1);
	xmm2 = _mm_add_ss(xmm2, Simd_one);
	xmm2 = _mm_mul_ss(xmm2, xmm0);

	xmm1 = _mm_mul_ss(Simd_MinusTwo, xmm2);
	xmm1 = _mm_add_ss(xmm1, Simd_HalfPi);
	xmm1 = _mm_and_ps(xmm1, xmm4);
	xmm2 = _mm_and_ps(xmm2, xmm3);
	xmm1 = _mm_add_ss(xmm1, xmm2);
	xmm1 = _mm_or_ps(xmm1, xmm5);				//asin(-x)=-asin(x)
	
	xmm2 = _mm_sub_ss(Simd_HalfPi, xmm1);

	_mm_store_ss(&f, xmm2);
	return f;
}

THOR_INLINE void AcosvOp(const ThSseVec4& arg, ThSseVec4& res)
{
	using namespace SSE;
	
	__m128 xmm0 = _mm_load_ps(arg.Begin());
	__m128 xmm1 = _mm_and_ps(xmm0, Simd_absMask);		//xmm1=abs(x)
	__m128 xmm5 = _mm_and_ps(xmm0, Simd_signBitMask);	//xmm5=signbit
	__m128 xmm2 = xmm1;
	__m128 xmm3 = _mm_sub_ps(Simd_one, xmm1);			//calc  xmm3=sqrt( (1-x)/2 )
	xmm3 = _mm_mul_ps(xmm3, Simd_half);
	xmm3 = _mm_sqrt_ps(xmm3);
	xmm1 = _mm_min_ps(xmm1, xmm3);			   //choose min
	xmm0 = xmm1;		   //set min as argument
	xmm2 = _mm_cmplt_ps(xmm2, Simd_half);			//is abs(x) > 0.5 
	xmm3 = xmm2;

	xmm2 = _mm_xor_ps(xmm2, Simd_not);
	__m128 xmm4 = xmm2;

	xmm1 = _mm_mul_ps(xmm1, xmm1);
	xmm2 = _mm_mul_ps(Simd_asin_c9, xmm1);
	xmm2 = _mm_add_ps(xmm2, Simd_asin_c8);
	xmm2 = _mm_mul_ps(xmm2, xmm1);
	xmm2 = _mm_add_ps(xmm2, Simd_asin_c7);
	xmm2 = _mm_mul_ps(xmm2, xmm1);
	xmm2 = _mm_add_ps(xmm2, Simd_asin_c6);
	xmm2 = _mm_mul_ps(xmm2, xmm1);
	xmm2 = _mm_add_ps(xmm2, Simd_asin_c5);
	xmm2 = _mm_mul_ps(xmm2, xmm1);
	xmm2 = _mm_add_ps(xmm2, Simd_asin_c4);
	xmm2 = _mm_mul_ps(xmm2, xmm1);
	xmm2 = _mm_add_ps(xmm2, Simd_asin_c3);
	xmm2 = _mm_mul_ps(xmm2, xmm1);
	xmm2 = _mm_add_ps(xmm2, Simd_asin_c2);
	xmm2 = _mm_mul_ps(xmm2, xmm1);
	xmm2 = _mm_add_ps(xmm2, Simd_asin_c1);
	xmm2 = _mm_mul_ps(xmm2, xmm1);
	xmm2 = _mm_add_ps(xmm2, Simd_asin_c0);
	xmm2 = _mm_mul_ps(xmm2, xmm1);
	xmm2 = _mm_add_ps(xmm2, Simd_one);
	xmm2 = _mm_mul_ps(xmm2, xmm0);

	xmm1 = _mm_mul_ps(Simd_MinusTwo, xmm2);
	xmm1 = _mm_add_ps(xmm1, Simd_HalfPi);
	xmm1 = _mm_and_ps(xmm1, xmm4);
	xmm2 = _mm_and_ps(xmm2, xmm3);
	xmm1 = _mm_add_ps(xmm1, xmm2);
	xmm1 = _mm_or_ps(xmm1, xmm5);				//asin(-x)=-asin(x)

	xmm2 = _mm_sub_ps(Simd_HalfPi, xmm1);

	_mm_store_ps(res.Begin(), xmm2);
}

THOR_INLINE float Sign(float f)
{
	using namespace SSE;
	__m128 xmm0 = _mm_load_ss(&f);
	xmm0 = _mm_and_ps(xmm0, Simd_signBitMask);
	xmm0 = _mm_xor_ps(Simd_one, xmm0);

	_mm_store_ss(&f, xmm0);
	return f;
}

THOR_INLINE void SignvOp(const ThSseVec4& arg, ThSseVec4& res)
{
	using namespace SSE;
	__m128 xmm0 = _mm_load_ps(arg.Begin());
	xmm0 = _mm_and_ps(xmm0, Simd_signBitMask);
	xmm0 = _mm_xor_ps(Simd_one, xmm0);

	_mm_store_ps(res.Begin(), xmm0);
}

THOR_INLINE float Abs(float f)
{
	using namespace SSE;
	__m128 xmm0 = _mm_load_ss(&f);
	xmm0 = _mm_and_ps(xmm0, Simd_absMask);

	_mm_store_ss(&f, xmm0);
	return f;
}

THOR_INLINE void AbsvOp(const ThSseVec4& arg, ThSseVec4& res)
{
	using namespace SSE;
	__m128 xmm0 = _mm_load_ps(arg.Begin());
	xmm0 = _mm_and_ps(xmm0, Simd_absMask);
	_mm_store_ps(res.Begin(), xmm0);
}

THOR_INLINE float Floor(float f)
{
	using namespace SSE;
	__m128 xmm0 = _mm_load_ss(&f);
	xmm0 = _mm_floor_ss(xmm0, xmm0);
	_mm_store_ss(&f, xmm0);
	return f;
}

THOR_INLINE void FloorvOp(const ThSseVec4& arg, ThSseVec4& res)
{
	using namespace SSE;
	__m128 xmm0 = _mm_load_ps(arg.Begin());
	xmm0 = _mm_floor_ps(xmm0);
	_mm_store_ps(res.Begin(), xmm0);
}

THOR_INLINE float Ceil(float f)
{
	using namespace SSE;
	__m128 xmm0 = _mm_load_ss(&f);
	xmm0 = _mm_ceil_ss(xmm0, xmm0);
	_mm_store_ss(&f, xmm0);
	return f;
}

THOR_INLINE void CeilvOp(const ThSseVec4& arg, ThSseVec4& res)
{
	using namespace SSE;
	__m128 xmm0 = _mm_load_ps(arg.Begin());
	xmm0 = _mm_ceil_ps(xmm0);
	_mm_store_ps(res.Begin(), xmm0);
}

THOR_INLINE float Clamp(float f, float min, float max)
{
	using namespace SSE;
	__m128 xmm0 = _mm_load_ss(&f);
	__m128 xmm1 = _mm_load_ss(&min);
	__m128 xmm2 = _mm_load_ss(&max);
	xmm0 = _mm_max_ps(xmm0, xmm1);
	xmm0 = _mm_min_ps(xmm0, xmm2);

	_mm_store_ss(&f, xmm0);
	return f;
}

THOR_INLINE void ClampvOp(const ThSseVec4& arg, float min, float max, ThSseVec4& res)
{
	using namespace SSE;
	__m128 xmm0 = _mm_load_ps(arg.Begin());
	__m128 xmm1 = _mm_load_ss(&min);
	__m128 xmm2 = _mm_load_ss(&max);
	xmm1 = _mm_shuffle_ps(xmm1, xmm1, 0);
	xmm2 = _mm_shuffle_ps(xmm2, xmm2, 0);
	xmm0 = _mm_max_ps(xmm0, xmm1);
	xmm0 = _mm_min_ps(xmm0, xmm2);

	_mm_store_ps(res.Begin(), xmm0);
}

THOR_INLINE float  Pwr2(int p)
{
	//computes 2^p by moving p into exponent bits of a float
	float pw;
	using namespace SSE;
	__m128i xmm0 = _mm_set_epi32(p, p, p, p);
	xmm0 = _mm_add_epi32(xmm0, _mm_set_epi32(127, 127, 127, 127));	//bias exponent
	xmm0 = _mm_slli_epi32(xmm0, 23);				//move bits to an exponent part of a float

	_mm_store_ss(&pw, _mm_castsi128_ps(xmm0));
	return pw;
}

THOR_INLINE void Pwr2vOp(const ThSseVec4i& pwr, ThSseVec4& res)
{
	using namespace SSE;
	__m128i xmm0 = _mm_set_epi32(pwr(3), pwr(2), pwr(1), pwr(0));
	xmm0 = _mm_add_epi32(xmm0, _mm_set_epi32(127, 127, 127, 127));	//bias exponent
	xmm0 = _mm_slli_epi32(xmm0, 23);				//move bits to an exponent part of a float

	_mm_store_ps(res.Begin(), _mm_castsi128_ps(xmm0));
}

THOR_INLINE void  MantissaExp(float f, float &m, float &e, float &p)
{
	//extracts	mantissa and exponent from f and computes 2^e
	using namespace SSE;
	__m128 xmm0 = _mm_load_ss(&f);
	__m128 xmm1 = _mm_and_ps(xmm0, Simd_IEEE_FEXPMASKt); //extract  exponent 
	xmm0 = _mm_or_ps(xmm0, Simd_IEEE_FMNTMASKt);		//extract	mantissa  it has exponent 01111111 if f is normalized
	xmm0 = _mm_and_ps(xmm0, Simd_IEEE_FMNTMASKt1);		//remove highest bit from mantissa   e=0..127 => f<=1 e>127 => f>1		

	_mm_store_ss(&m, xmm0);						//return mantissa
	_mm_store_ss(&p, xmm1);						//return 2^exp

	__m128i cvt = _mm_castps_si128(xmm1);
	cvt = _mm_srli_epi32(cvt, 23);
	cvt = _mm_sub_epi32(cvt, _mm_set_epi32(127, 127, 127, 127)); //unbias exponent e=e-127
	xmm1 = _mm_cvtepi32_ps(cvt);

	_mm_store_ss(&e, xmm1);
}

THOR_INLINE float Exp(float f)
{
	using namespace SSE;
	// e^x=2^(x/ln2)=2^(k.f)=2^k*2^f=2^k*e^(f*ln2)		

	__m128 xmm0 = _mm_load_ss(&f);
	xmm0 = _mm_mul_ss(xmm0, Simd_1divLn2);		//xmm0=x/ln2
	__m128 xmm1 = _mm_sub_ss(xmm0, Simd_half);		//round down
	__m128i cvt0 = _mm_cvtps_epi32(xmm1);				//xmm1=k cvt0->xmm1
	xmm1 = _mm_cvtepi32_ps(cvt0);
						//compute 2^k
	cvt0 = _mm_add_epi32(cvt0, _mm_set_epi32(127, 127, 127, 127));	//unbias exponent
	cvt0 = _mm_slli_epi32(cvt0, 23);				//move bits to an exponent part of a float				
	__m128 xmm2 = _mm_castsi128_ps(cvt0);

	xmm0 = _mm_sub_ss(xmm0, xmm1);
	xmm0 = _mm_mul_ss(xmm0, Simd_Ln2);		//calc f*ln2

	__m128 xmm4 = _mm_mul_ss(Simd_exp_c6, xmm0);
	xmm4 = _mm_add_ss(xmm4, Simd_exp_c5);
	xmm4 = _mm_mul_ss(xmm4, xmm0);
	xmm4 = _mm_add_ss(xmm4, Simd_exp_c4);
	xmm4 = _mm_mul_ss(xmm4, xmm0);
	xmm4 = _mm_add_ss(xmm4, Simd_exp_c3);
	xmm4 = _mm_mul_ss(xmm4, xmm0);
	xmm4 = _mm_add_ss(xmm4, Simd_exp_c2);
	xmm4 = _mm_mul_ss(xmm4, xmm0);
	xmm4 = _mm_add_ss(xmm4, Simd_exp_c1);
	xmm4 = _mm_mul_ss(xmm4, xmm0);
	xmm4 = _mm_add_ss(xmm4, Simd_exp_c0);
	xmm4 = _mm_mul_ss(xmm4, xmm0);
	xmm4 = _mm_add_ss(xmm4, Simd_one);
	xmm4 = _mm_mul_ss(xmm4, xmm0);
	xmm4 = _mm_add_ss(xmm4, Simd_one);

	xmm4 = _mm_mul_ss(xmm4, xmm2);

	_mm_store_ss(&f, xmm4);
	return f;
}


THOR_INLINE void ExpvOp(const ThSseVec4& arg, ThSseVec4& res)
{
	using namespace SSE;
	// e^x=2^(x/ln2)=2^(k.f)=2^k*2^f=2^k*e^(f*ln2)		

	__m128 xmm0 = _mm_load_ps(arg.Begin());
	xmm0 = _mm_mul_ps(xmm0, Simd_1divLn2);		//xmm0=x/ln2
	__m128 xmm1 = _mm_sub_ps(xmm0, Simd_half);		//round down
	__m128i cvt0 = _mm_cvtps_epi32(xmm1);				//xmm1=k cvt0->xmm1
	xmm1 = _mm_cvtepi32_ps(cvt0);
	//compute 2^k
	cvt0 = _mm_add_epi32(cvt0, _mm_set_epi32(127, 127, 127, 127));	//unbias exponent
	cvt0 = _mm_slli_epi32(cvt0, 23);				//move bits to an exponent part of a float				
	__m128 xmm2 = _mm_castsi128_ps(cvt0);

	xmm0 = _mm_sub_ps(xmm0, xmm1);
	xmm0 = _mm_mul_ps(xmm0, Simd_Ln2);		//calc f*ln2

	__m128 xmm4 = _mm_mul_ps(Simd_exp_c6, xmm0);
	xmm4 = _mm_add_ps(xmm4, Simd_exp_c5);
	xmm4 = _mm_mul_ps(xmm4, xmm0);
	xmm4 = _mm_add_ps(xmm4, Simd_exp_c4);
	xmm4 = _mm_mul_ps(xmm4, xmm0);
	xmm4 = _mm_add_ps(xmm4, Simd_exp_c3);
	xmm4 = _mm_mul_ps(xmm4, xmm0);
	xmm4 = _mm_add_ps(xmm4, Simd_exp_c2);
	xmm4 = _mm_mul_ps(xmm4, xmm0);
	xmm4 = _mm_add_ps(xmm4, Simd_exp_c1);
	xmm4 = _mm_mul_ps(xmm4, xmm0);
	xmm4 = _mm_add_ps(xmm4, Simd_exp_c0);
	xmm4 = _mm_mul_ps(xmm4, xmm0);
	xmm4 = _mm_add_ps(xmm4, Simd_one);
	xmm4 = _mm_mul_ps(xmm4, xmm0);
	xmm4 = _mm_add_ps(xmm4, Simd_one);

	xmm4 = _mm_mul_ps(xmm4, xmm2);

	_mm_store_ps(res.Begin(), xmm4);
}

THOR_INLINE float Ln(float f)
{
	using namespace SSE;
	//ln(x)=ln(m*2^exp)=ln(m)+exp*ln 2
	//t=(m-1)/(m+1);
	//float t2=t*t;
	//return 2*t*(1.0+t2*(1/3.0+t2*(1/5.0+1/7.0*t2)))+e*log(2.0f);

	__m128 xmm0 = _mm_load_ss(&f);
	__m128 neg = _mm_cmple_ss(xmm0, Simd_zero);
	__m128 xmm1 = _mm_and_ps(xmm0, Simd_IEEE_FEXPMASKt); //extract  exponent 
	xmm0 = _mm_or_ps(xmm0, Simd_IEEE_FMNTMASKt);		//extract	mantissa  it has exponent 01111111 if f is normalized
	xmm0 = _mm_and_ps(xmm0, Simd_IEEE_FMNTMASKt1);		//remove highest bit from mantissa   e=0..127 => f<=1 e>127 => f>1		

	__m128 xmm2 =  _mm_add_ss(xmm0, Simd_one);						//xmm2 = m+1
	__m128i cvt = _mm_castps_si128(xmm1);
	cvt = _mm_srli_epi32(cvt, 23);
	cvt = _mm_sub_epi32(cvt, _mm_set_epi32(127, 127, 127, 127)); //unbias exponent e=e-127
	xmm1 = _mm_cvtepi32_ps(cvt);
			
	xmm0 = _mm_sub_ss(xmm0, Simd_one);					//xmm0=m-1
	xmm0 = _mm_div_ss(xmm0, xmm2);						//xmm0=t=(m-1)/(m+1)
	xmm1 = _mm_mul_ss(xmm1, Simd_Ln2);					//xmm1=exp*ln2

	xmm2 = _mm_mul_ss(xmm0, xmm0);						//xmm2=t*t

	__m128 xmm3 = _mm_mul_ss(Simd_ln_c3, xmm2);			//xmm3=1/7*t2
	xmm3 = _mm_add_ss(xmm3, Simd_ln_c2);
	xmm3 = _mm_mul_ss(xmm3, xmm2);						//xmm3=t2*(1/5+1/7*t2)
	xmm3 = _mm_add_ss(xmm3, Simd_ln_c1);
	xmm3 = _mm_mul_ss(xmm3, xmm2);						//xmm3=t2*(1/3+t2*(1/5+1/7*t2))
	xmm3 = _mm_add_ss(xmm3, Simd_ln_c0);
	xmm3 = _mm_mul_ss(xmm3, xmm0);						//xmm3=t*(1+t2*(1/3+t2*(1/5+1/7*t2)))
	xmm3 = _mm_add_ss(xmm3, xmm3);						//xmm3=2*t*(1+t2*(1/3+t2*(1/5+1/7*t2)))

	xmm3 = _mm_add_ss(xmm3, xmm1);
	xmm3 = Blend4(neg, Simd_NaN, xmm3);
	_mm_store_ss(&f, xmm3);
	return f;
}

THOR_INLINE void LnvOp(const ThSseVec4& arg, ThSseVec4& res)
{
	using namespace SSE;

	__m128 xmm0 = _mm_load_ps(arg.Begin());
	__m128 neg = _mm_cmple_ps(xmm0, Simd_zero);
	__m128 xmm1 = _mm_and_ps(xmm0, Simd_IEEE_FEXPMASKt); //extract  exponent 
	xmm0 = _mm_or_ps(xmm0, Simd_IEEE_FMNTMASKt);		//extract	mantissa  it has exponent 01111111 if f is normalized
	xmm0 = _mm_and_ps(xmm0, Simd_IEEE_FMNTMASKt1);		//remove highest bit from mantissa   e=0..127 => f<=1 e>127 => f>1		

	__m128 xmm2 = _mm_add_ps(xmm0, Simd_one);						//xmm2 = m+1
	__m128i cvt = _mm_castps_si128(xmm1);
	cvt = _mm_srli_epi32(cvt, 23);
	cvt = _mm_sub_epi32(cvt, _mm_set_epi32(127, 127, 127, 127)); //unbias exponent e=e-127
	xmm1 = _mm_cvtepi32_ps(cvt);

	xmm0 = _mm_sub_ps(xmm0, Simd_one);					//xmm0=m-1
	xmm0 = _mm_div_ps(xmm0, xmm2);						//xmm0=t=(m-1)/(m+1)
	xmm1 = _mm_mul_ps(xmm1, Simd_Ln2);					//xmm1=exp*ln2

	xmm2 = _mm_mul_ps(xmm0, xmm0);						//xmm2=t*t

	__m128 xmm3 = _mm_mul_ps(Simd_ln_c3, xmm2);			//xmm3=1/7*t2
	xmm3 = _mm_add_ps(xmm3, Simd_ln_c2);
	xmm3 = _mm_mul_ps(xmm3, xmm2);						//xmm3=t2*(1/5+1/7*t2)
	xmm3 = _mm_add_ps(xmm3, Simd_ln_c1);
	xmm3 = _mm_mul_ps(xmm3, xmm2);						//xmm3=t2*(1/3+t2*(1/5+1/7*t2))
	xmm3 = _mm_add_ps(xmm3, Simd_ln_c0);
	xmm3 = _mm_mul_ps(xmm3, xmm0);						//xmm3=t*(1+t2*(1/3+t2*(1/5+1/7*t2)))
	xmm3 = _mm_add_ps(xmm3, xmm3);						//xmm3=2*t*(1+t2*(1/3+t2*(1/5+1/7*t2)))

	xmm3 = _mm_add_ps(xmm3, xmm1);	
	xmm3 = Blend4(neg, Simd_NaN, xmm3);
	_mm_store_ps(res.Begin(), xmm3);
}

}}