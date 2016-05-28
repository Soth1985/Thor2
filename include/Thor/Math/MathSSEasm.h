#pragma once

#include <Thor/Framework/Common.h>
#include <Thor/Math/Constants.h>
#include <Thor/Math/FixedVectorFwd.h>

namespace Thor{ namespace Math{

THOR_INLINE float Sqrt(float f)
{
	__asm
	{
		movss  xmm0, f		
		sqrtss xmm1, xmm0
		movss  f,  xmm1
	}
	return f;
}

THOR_INLINE void SqrtvOp(const ThSseVec4& arg, ThSseVec4& res)
{
	__asm
	{
		mov    edi,   arg
		mov	   esi,   res	
		movaps xmm0,  [edi]
		sqrtps xmm1,  xmm0
		movaps [esi], xmm1
	}
}

THOR_INLINE float InvSqrt(float f)
{	
	__asm
	{
		movss		xmm0, f
		rsqrtss		xmm1, xmm0
		movss		f, xmm1
	}
	return f;
}

THOR_INLINE void InvSqrtvOp(const ThSseVec4& arg, ThSseVec4& res)
{
	__asm
	{
		mov     edi,   arg
		mov     esi,   res
		movaps  xmm0,  [edi]
		rsqrtps xmm1,  xmm0
		movaps  [esi], xmm1
	}
}

THOR_INLINE float Sin(float f)
{
	using namespace SSE;
	__asm
	{
		movss    xmm0, f
		movss	 xmm1, xmm0
		mulss    xmm1, Simd_1div2Pi		//xmm1=a/(2pi)
		subss    xmm1, Simd_half		// begin cutoff fractional part  xmm1=floor(xmm1)
		cvtss2si eax,  xmm1 
		cvtsi2ss xmm1, eax				// end cutoff fractional part
		mulss    xmm1, Simd_2Pi
		subss    xmm0, xmm1				//xmm0 now is between 0 and -+2pi

		
		movss		xmm1, Simd_Pi				// xmm1 = PI
		subss		xmm1, xmm0					// xmm1 = PI - a
		movss		xmm0, xmm1					// xmm0 = PI - a
		andps		xmm0, Simd_signBitMask		// xmm0 = signbit( PI - a )
		movss		xmm2, xmm1					// xmm2 = PI - a
		xorps		xmm2, xmm0					// xmm2 = fabs( PI - a )
		cmpnltss	xmm2, Simd_HalfPi			// xmm2 = ( fabs( PI - a ) >= HALF_PI ) ? 0xFFFFFFFF : 0x00000000
		movss		xmm3, Simd_Pi				// xmm3 = PI
		xorps		xmm3, xmm0					// xmm3 = PI xor signbit( PI - a )
		andps		xmm3, xmm2					// xmm3 = ( fabs( PI - a ) >= HALF_PI ) ? ( PI xor signbit( PI - a ) ) : 0.0f
		andps		xmm2, Simd_signBitMask		// xmm2 = ( fabs( PI - a ) >= HALF_PI ) ? Simd_signBitMask : 0.0f
		xorps		xmm1, xmm2
		addps		xmm1, xmm3

		movss		xmm0, xmm1				//x
		mulss		xmm0, xmm0				//xmm0=x^2
		movss		xmm2, Simd_SinC4
		mulss		xmm2, xmm0				//xmm2=c4*x^2
		addss		xmm2, Simd_SinC3		//xmm2=c4*x^2+c3
		mulss		xmm2, xmm0				//xmm2=x^2*(c4*x^2+c3)
		addss		xmm2, Simd_SinC2		//xmm2=c2+x^2*(c4*x^2+c3)
		mulss		xmm2, xmm0				//xmm2=x^2*(c2+x^2*(c4*x^2+c3))
		addss		xmm2, Simd_SinC1		//xmm2=c1+x^2*(c2+x^2*(c4*x^2+c3))
		mulss		xmm2, xmm0				//xmm2=x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3)))
		addss		xmm2, Simd_SinC0		//xmm2=c0+x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3)))
		mulss		xmm2, xmm0				//xmm2=x^2*(c0+x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3))))
		addss		xmm2, Simd_one			//xmm2=1.0+x^2*(c0+x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3))))
		mulss		xmm2, xmm1				//xmm2=x*(1.0+x^2*(c0+x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3)))))

		movss    f,xmm2		
	}

	return f;
}

THOR_INLINE void SinvOp(const ThSseVec4& arg, ThSseVec4& res)
{
	using namespace SSE;
	__asm
	{
		mov			edi, arg
		mov			esi, res 
		movaps		xmm0, [edi]
		movaps		xmm1, xmm0
		mulps		xmm1, Simd_1div2Pi	//xmm1=a/(2pi)
		subps		xmm1, Simd_half		// begin cutoff fractional part  xmm1=floor(xmm1)
		cvtps2dq	xmm3, xmm1			//sse2
		cvtdq2ps	xmm1, xmm3			// end cutoff fractional part
		mulps		xmm1, Simd_2Pi
		subps		xmm0, xmm1			//xmm0 now is between 0 and -+2pi

		
		movaps		xmm1, Simd_Pi				// xmm1 = PI
		subps		xmm1, xmm0					// xmm1 = PI - a
		movaps		xmm0, xmm1					// xmm0 = PI - a
		andps		xmm0, Simd_signBitMask		// xmm0 = signbit( PI - a )
		movaps		xmm2, xmm1					// xmm2 = PI - a
		xorps		xmm2, xmm0					// xmm2 = fabs( PI - a )
		cmpnltps	xmm2, Simd_HalfPi			// xmm2 = ( fabs( PI - a ) >= HALF_PI ) ? 0xFFFFFFFF : 0x00000000
		movaps		xmm3, Simd_Pi				// xmm3 = PI
		xorps		xmm3, xmm0					// xmm3 = PI xor signbit( PI - a )
		andps		xmm3, xmm2					// xmm3 = ( fabs( PI - a ) >= HALF_PI ) ? ( PI xor signbit( PI - a ) ) : 0.0f
		andps		xmm2, Simd_signBitMask		// xmm2 = ( fabs( PI - a ) >= HALF_PI ) ? Simd_signBitMask : 0.0f
		xorps		xmm1, xmm2
		addps		xmm1, xmm3

		movaps		xmm0, xmm1				//x
		mulps		xmm0, xmm0				//xmm0=x^2
		movaps		xmm2, Simd_SinC4
		mulps		xmm2, xmm0				//xmm2=c4*x^2
		addps		xmm2, Simd_SinC3		//xmm2=c4*x^2+c3
		mulps		xmm2, xmm0				//xmm2=x^2*(c4*x^2+c3)
		addps		xmm2, Simd_SinC2		//xmm2=c2+x^2*(c4*x^2+c3)
		mulps		xmm2, xmm0				//xmm2=x^2*(c2+x^2*(c4*x^2+c3))
		addps		xmm2, Simd_SinC1		//xmm2=c1+x^2*(c2+x^2*(c4*x^2+c3))
		mulps		xmm2, xmm0				//xmm2=x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3)))
		addps		xmm2, Simd_SinC0		//xmm2=c0+x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3)))
		mulps		xmm2, xmm0				//xmm2=x^2*(c0+x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3))))
		addps		xmm2, Simd_one			//xmm2=1.0+x^2*(c0+x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3))))
		mulps		xmm2, xmm1				//xmm2=x*(1.0+x^2*(c0+x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3)))))

		movaps		[esi],xmm2		
	}
	
}

THOR_INLINE float Cos(float f)
{
	using namespace SSE;
	__asm
	{
		movss    xmm0, f
		movss	 xmm1, xmm0
		mulss    xmm1, Simd_1div2Pi		//xmm1=a/(2pi)
		subss    xmm1, Simd_half		// begin cutoff fractional part  xmm1=floor(xmm1)
		cvtss2si eax, xmm1 
		cvtsi2ss xmm1, eax				// end cutoff fractional part
		mulss    xmm1, Simd_2Pi
		subss    xmm0, xmm1				//xmm0 now is between 0 and -+2pi

		
		movss		xmm1, Simd_Pi				// xmm1 = PI
		subss		xmm1, xmm0					// xmm1 = PI - a
		movss		xmm0, xmm1					// xmm0 = PI - a
		andps		xmm0, Simd_signBitMask		// xmm0 = signbit( PI - a )
		movss		xmm2, xmm1					// xmm2 = PI - a
		xorps		xmm2, xmm0					// xmm2 = fabs( PI - a )
		cmpnltss	xmm2, Simd_HalfPi			// xmm2 = ( fabs( PI - a ) >= HALF_PI ) ? 0xFFFFFFFF : 0x00000000
		movss		xmm3, Simd_Pi				// xmm3 = PI
		xorps		xmm3, xmm0					// xmm3 = PI xor signbit( PI - a )
		andps		xmm3, xmm2					// xmm3 = ( fabs( PI - a ) >= HALF_PI ) ? ( PI xor signbit( PI - a ) ) : 0.0f
		andps		xmm2, Simd_signBitMask		// xmm2 = ( fabs( PI - a ) >= HALF_PI ) ? Simd_signBitMask : 0.0f
		xorps		xmm1, xmm2
		addps		xmm1, xmm3

		

		movss		xmm0, xmm1				//x
		mulss		xmm0, xmm0				//xmm0=x^2
		movss		xmm3, Simd_CosC4
		mulss		xmm3, xmm0				//xmm3=c4*x^2
		addss		xmm3, Simd_CosC3		//xmm3=c4*x^2+c3
		mulss		xmm3, xmm0				//xmm3=x^2*(c4*x^2+c3)
		addss		xmm3, Simd_CosC2		//xmm3=c2+x^2*(c4*x^2+c3)
		mulss		xmm3, xmm0				//xmm3=x^2*(c2+x^2*(c4*x^2+c3))
		addss		xmm3, Simd_CosC1		//xmm3=c1+x^2*(c2+x^2*(c4*x^2+c3))
		mulss		xmm3, xmm0				//xmm3=x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3)))
		addss		xmm3, Simd_CosC0		//xmm3=c0+x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3)))
		mulss		xmm3, xmm0				//xmm3=x^2*(c0+x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3))))
		addss		xmm3, Simd_one			//xmm3=1.0+x^2*(c0+x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3))))
		xorps		xmm2, Simd_signBitMask
		xorps		xmm3, xmm2

		movss    f,xmm3
	}
	return f;

}

////////////////////////////////////////////////////////////////////////////////
THOR_INLINE void CosvOp(const ThSseVec4& arg, ThSseVec4& res)
{
	using namespace SSE;
	__asm
	{
		mov			edi, arg
		mov			esi, res 
		movaps		xmm0, [edi]
		movaps		xmm1, xmm0
		mulps		xmm1, Simd_1div2Pi	//xmm1=a/(2pi)
		subps		xmm1, Simd_half		// begin cutoff fractional part  xmm1=floor(xmm1)
		cvtps2dq	xmm3, xmm1			//sse2
		cvtdq2ps	xmm1, xmm3			// end cutoff fractional part
		mulps		xmm1, Simd_2Pi
		subps		xmm0, xmm1			//xmm0 now is between 0 and -+2pi

		
		movaps		xmm1, Simd_Pi				// xmm1 = PI
		subps		xmm1, xmm0					// xmm1 = PI - a
		movaps		xmm0, xmm1					// xmm0 = PI - a
		andps		xmm0, Simd_signBitMask		// xmm0 = signbit( PI - a )
		movaps		xmm2, xmm1					// xmm2 = PI - a
		xorps		xmm2, xmm0					// xmm2 = fabs( PI - a )
		cmpnltps	xmm2, Simd_HalfPi			// xmm2 = ( fabs( PI - a ) >= HALF_PI ) ? 0xFFFFFFFF : 0x00000000
		movaps		xmm3, Simd_Pi				// xmm3 = PI
		xorps		xmm3, xmm0					// xmm3 = PI xor signbit( PI - a )
		andps		xmm3, xmm2					// xmm3 = ( fabs( PI - a ) >= HALF_PI ) ? ( PI xor signbit( PI - a ) ) : 0.0f
		andps		xmm2, Simd_signBitMask		// xmm2 = ( fabs( PI - a ) >= HALF_PI ) ? Simd_signBitMask : 0.0f
		xorps		xmm1, xmm2
		addps		xmm1, xmm3

		

		movaps		xmm0, xmm1				//x
		mulps		xmm0, xmm0				//xmm0=x^2
		movaps		xmm3, Simd_CosC4
		mulps		xmm3, xmm0				//xmm2=c4*x^2
		addps		xmm3, Simd_CosC3		//xmm2=c4*x^2+c3
		mulps		xmm3, xmm0				//xmm2=x^2*(c4*x^2+c3)
		addps		xmm3, Simd_CosC2		//xmm2=c2+x^2*(c4*x^2+c3)
		mulps		xmm3, xmm0				//xmm2=x^2*(c2+x^2*(c4*x^2+c3))
		addps		xmm3, Simd_CosC1		//xmm2=c1+x^2*(c2+x^2*(c4*x^2+c3))
		mulps		xmm3, xmm0				//xmm2=x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3)))
		addps		xmm3, Simd_CosC0		//xmm2=c0+x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3)))
		mulps		xmm3, xmm0				//xmm2=x^2*(c0+x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3))))
		addps		xmm3, Simd_one			//xmm2=1.0+x^2*(c0+x^2*(c1+x^2*(c2+x^2*(c4*x^2+c3))))
		xorps		xmm2, Simd_signBitMask
		xorps		xmm3, xmm2

		movaps		[esi],xmm3
	}	
}


THOR_INLINE void SinCos(float f,float &s,float &c)
{
	using namespace SSE;
	__asm
	{
		mov			edi, s
		mov			esi, c
		movss		xmm0, f
		movss		xmm1, xmm0
		mulss		xmm1, Simd_1div2Pi		//xmm1=a/(2pi)
		subss		xmm1, Simd_half			// begin cutoff fractional part  xmm1=floor(xmm1)
		cvtss2si	eax, xmm1 
		cvtsi2ss	xmm1, eax				// end cutoff fractional part
		mulss		xmm1, Simd_2Pi
		subss		xmm0, xmm1				//xmm0 now is between 0 and -+2pi

		
		movss		xmm1, Simd_Pi				// xmm1 = PI
		subss		xmm1, xmm0					// xmm1 = PI - a
		movss		xmm0, xmm1					// xmm0 = PI - a
		andps		xmm0, Simd_signBitMask		// xmm0 = signbit( PI - a )
		movss		xmm2, xmm1					// xmm2 = PI - a
		xorps		xmm2, xmm0					// xmm2 = fabs( PI - a )
		cmpnltss	xmm2, Simd_HalfPi			// xmm2 = ( fabs( PI - a ) >= HALF_PI ) ? 0xFFFFFFFF : 0x00000000
		movss		xmm3, Simd_Pi				// xmm3 = PI
		xorps		xmm3, xmm0					// xmm3 = PI xor signbit( PI - a )
		andps		xmm3, xmm2					// xmm3 = ( fabs( PI - a ) >= HALF_PI ) ? ( PI xor signbit( PI - a ) ) : 0.0f
		andps		xmm2, Simd_signBitMask		// xmm2 = ( fabs( PI - a ) >= HALF_PI ) ? Simd_signBitMask : 0.0f
		xorps		xmm1, xmm2
		addps		xmm1, xmm3

		

		movss		xmm0, xmm1				
		mulss		xmm0, xmm0				
		movss		xmm3, Simd_CosC4
		movss		xmm4, Simd_SinC4
		mulss		xmm3, xmm0				
		mulss		xmm4, xmm0
		addss		xmm3, Simd_CosC3		
		addss		xmm4, Simd_SinC3		
		mulss		xmm3, xmm0
		mulss		xmm4, xmm0
		addss		xmm3, Simd_CosC2		
		addss		xmm4, Simd_SinC2		
		mulss		xmm3, xmm0
		mulss		xmm4, xmm0
		addss		xmm3, Simd_CosC1		
		addss		xmm4, Simd_SinC1		
		mulss		xmm3, xmm0
		mulss		xmm4, xmm0
		addss		xmm3, Simd_CosC0		
		addss		xmm4, Simd_SinC0		
		mulss		xmm3, xmm0
		mulss		xmm4, xmm0
		addss		xmm3, Simd_one
		addss		xmm4, Simd_one
		mulss		xmm4, xmm1
		xorps		xmm2, Simd_signBitMask
		xorps		xmm3, xmm2

		movss    [edi],xmm4
		movss    [esi],xmm3
	}	
}

THOR_INLINE void SinCosvOp(const ThSseVec4& arg, ThSseVec4& s, ThSseVec4& c)
{
	using namespace SSE;
	__asm
	{
		mov			edi, s
		mov			esi, c
		mov			edx, arg
		movaps		xmm0, [edx]
		movaps		xmm1, xmm0
		mulps		xmm1, Simd_1div2Pi		//xmm1=a/(2pi)
		subps		xmm1, Simd_half			// begin cutoff fractional part  xmm1=floor(xmm1)
		cvtps2dq	xmm3, xmm1 
		cvtdq2ps	xmm1, xmm3				// end cutoff fractional part
		mulps		xmm1, Simd_2Pi
		subps		xmm0, xmm1				//xmm0 now is between 0 and -+2pi

		
		movaps		xmm1, Simd_Pi				// xmm1 = PI
		subps		xmm1, xmm0					// xmm1 = PI - a
		movaps		xmm0, xmm1					// xmm0 = PI - a
		andps		xmm0, Simd_signBitMask		// xmm0 = signbit( PI - a )
		movaps		xmm2, xmm1					// xmm2 = PI - a
		xorps		xmm2, xmm0					// xmm2 = fabs( PI - a )
		cmpnltps	xmm2, Simd_HalfPi			// xmm2 = ( fabs( PI - a ) >= HALF_PI ) ? 0xFFFFFFFF : 0x00000000
		movaps		xmm3, Simd_Pi				// xmm3 = PI
		xorps		xmm3, xmm0					// xmm3 = PI xor signbit( PI - a )
		andps		xmm3, xmm2					// xmm3 = ( fabs( PI - a ) >= HALF_PI ) ? ( PI xor signbit( PI - a ) ) : 0.0f
		andps		xmm2, Simd_signBitMask		// xmm2 = ( fabs( PI - a ) >= HALF_PI ) ? Simd_signBitMask : 0.0f
		xorps		xmm1, xmm2
		addps		xmm1, xmm3		

		movaps		xmm0, xmm1				
		mulps		xmm0, xmm0				
		movaps		xmm3, Simd_CosC4
		movaps		xmm4, Simd_SinC4
		mulps		xmm3, xmm0				
		mulps		xmm4, xmm0
		addps		xmm3, Simd_CosC3		
		addps		xmm4, Simd_SinC3		
		mulps		xmm3, xmm0
		mulps		xmm4, xmm0
		addps		xmm3, Simd_CosC2		
		addps		xmm4, Simd_SinC2		
		mulps		xmm3, xmm0
		mulps		xmm4, xmm0
		addps		xmm3, Simd_CosC1		
		addps		xmm4, Simd_SinC1		
		mulps		xmm3, xmm0
		mulps		xmm4, xmm0
		addps		xmm3, Simd_CosC0		
		addps		xmm4, Simd_SinC0		
		mulps		xmm3, xmm0
		mulps		xmm4, xmm0
		addps		xmm3, Simd_one
		addps		xmm4, Simd_one
		mulps		xmm4, xmm1
		xorps		xmm2, Simd_signBitMask
		xorps		xmm3, xmm2

		movaps    [edi],xmm4
		movaps    [esi],xmm3
	}	
}


THOR_INLINE float Atan( float f ) 
{
	using namespace SSE;
	//atan(x)=x/(1+0.28*x^2)   if   abs(x)<=1
	//atan(x)=pi/2-x/(0.28+x^2)   if   abs(x)>1  x>0
	//atan(x)=-pi/2+x/(0.28+x^2)   if   abs(x)>1  x<0
	//atan(x)=xmm6|(xmm4+x/ ( xmm2&(1+0.28*x^2) + xmm3&(0.28+x^2)))
	__asm 
	{
		movss	xmm0, f
		movss	xmm1, xmm0
		andps	xmm1, Simd_absMask		//xmm1=abs(x)		
		
		movss	xmm6, xmm0
		andps	xmm6, Simd_signBitMask	//xmm6=signbit

		movss	xmm0, xmm1
		
		movss   xmm2, Simd_one
		movss   xmm3, Simd_one
		movss	xmm4, Simd_HalfPi
		
		cmpleps xmm1, Simd_one			//abs(x)<=1 ? 0xffffffff:0.0 
		movss	xmm2, xmm1
		movss	xmm3, xmm1		
		xorps   xmm3, Simd_not
		andps	xmm4, xmm3		

		movss	xmm1, xmm0
		mulss   xmm1, xmm1
		movss   xmm5, xmm1
		mulss   xmm5, Simd_Atan
		addss   xmm5, Simd_one
		andps   xmm5, xmm2
		addss   xmm1, Simd_Atan
		andps   xmm1, xmm3
		subss   xmm5, xmm1
		
		rcpss   xmm2, xmm5
		mulss	xmm2, xmm0
		addss   xmm2, xmm4
		orps    xmm2, xmm6

		movss	f, xmm2				//28
		

		
	}

	return f;
}

THOR_INLINE void AtanvOp(const ThSseVec4& arg, ThSseVec4& res)
{	
	using namespace SSE;
	//atan(x)=x/(1+0.28*x^2)   if   abs(x)<=1
	//atan(x)=pi/2-x/(0.28+x^2)   if   abs(x)>1  x>0
	//atan(x)=-pi/2+x/(0.28+x^2)   if   abs(x)>1  x<0
	__asm 
	{
		mov		edi, arg
		mov		esi, res
	
		movaps	xmm0, [edi]
		movaps	xmm1, xmm0
		andps	xmm1, Simd_absMask		//xmm1=abs(x)		
		
		movaps	xmm6, xmm0
		andps	xmm6, Simd_signBitMask	//xmm6=signbit

		movaps	xmm0, xmm1
		
		movaps   xmm2, Simd_one
		movaps   xmm3, Simd_one
		movaps	xmm4, Simd_HalfPi
		
		cmpleps xmm1, Simd_one			//abs(x)<=1 ? 0xffffffff:0.0 
		movaps	xmm2, xmm1
		movaps	xmm3, xmm1		
		xorps   xmm3, Simd_not
		andps	xmm4, xmm3		

		movaps	xmm1, xmm0
		mulps   xmm1, xmm1
		movaps  xmm5, xmm1
		mulps   xmm5, Simd_Atan
		addps   xmm5, Simd_one
		andps   xmm5, xmm2
		addps   xmm1, Simd_Atan
		andps   xmm1, xmm3
		subps   xmm5, xmm1
		
		rcpps   xmm2, xmm5
		mulps	xmm2, xmm0
		addps   xmm2, xmm4
		orps    xmm2, xmm6

		movaps	[esi], xmm2				//30		
	}	
}


THOR_INLINE float Atan2( float y, float x )
{
	using namespace SSE;
	__asm 
	{
		movss		xmm0, x
		movss		xmm3, xmm0
		movss		xmm4, xmm0
		andps		xmm0, Simd_absMask
		movss		xmm1, y
		xorps		xmm4, xmm1
		andps		xmm1, Simd_absMask
		andps		xmm4, Simd_signBitMask
		minss		xmm0, xmm1
		maxss		xmm1, xmm3
		cmpeqss		xmm3, xmm0
		rcpss		xmm2, xmm1
		mulss		xmm1, xmm2
		mulss		xmm1, xmm2
		addss		xmm2, xmm2
		subss		xmm2, xmm1				// xmm2 = 1 / y or 1 / x
		mulss		xmm0, xmm2				// xmm0 = x / y or y / x
		xorps		xmm0, xmm4
		movss		xmm1, xmm3
		andps		xmm1, Simd_signBitMask
		xorps		xmm0, xmm1				// xmm0 = -x / y or y / x
		orps		xmm4, Simd_HalfPi	// xmm4 = +/- HALF_PI
		andps		xmm3, xmm4				// xmm3 = +/- HALF_PI or 0.0f
		movss		xmm1, xmm0
		mulss		xmm1, xmm1				// xmm1 = s
		movss		xmm2, Simd_atan_c0
		mulss		xmm2, xmm1
		addss		xmm2, Simd_atan_c1
		mulss		xmm2, xmm1
		addss		xmm2, Simd_atan_c2
		mulss		xmm2, xmm1
		addss		xmm2, Simd_atan_c3
		mulss		xmm2, xmm1
		addss		xmm2, Simd_atan_c4
		mulss		xmm2, xmm1
		addss		xmm2, Simd_atan_c5
		mulss		xmm2, xmm1
		addss		xmm2, Simd_atan_c6
		mulss		xmm2, xmm1
		addss		xmm2, Simd_atan_c7
		mulss		xmm2, xmm1
		addss		xmm2, Simd_one
		mulss		xmm2, xmm0
		addss		xmm2, xmm3
		movss		y, xmm2					//45
	}

	return y;
}

THOR_INLINE void Atan2vOp(const ThSseVec4& y, const ThSseVec4& x, ThSseVec4& res)
{
	using namespace SSE;
	/*if ( fabs( y ) > fabs( x ) ) {
		a = -x / y;
		d = HALF_PI;
		*((unsigned long *)&d) ^= ( *((unsigned long *)&x) ^ *((unsigned long *)&y) ) & (1<<31);
	} else {
		a = y / x;
		d = 0.0f;
	}*/
	

	__asm 
	{
		mov			edi, y
		mov			esi, x
		mov			edx, res
		movaps		xmm0, [esi]
		movaps		xmm3, xmm0
		movaps		xmm4, xmm0
		andps		xmm0, Simd_absMask
		movaps		xmm1, [edi]
		xorps		xmm4, xmm1
		andps		xmm1, Simd_absMask
		andps		xmm4, Simd_signBitMask
		minps		xmm0, xmm1
		maxps		xmm1, xmm3
		cmpeqps		xmm3, xmm0
		rcpps		xmm2, xmm1
		mulps		xmm1, xmm2
		mulps		xmm1, xmm2
		addps		xmm2, xmm2
		subps		xmm2, xmm1				// xmm2 = 1 / y or 1 / x
		mulps		xmm0, xmm2				// xmm0 = x / y or y / x
		xorps		xmm0, xmm4
		movaps		xmm1, xmm3
		andps		xmm1, Simd_signBitMask
		xorps		xmm0, xmm1				// xmm0 = -x / y or y / x
		orps		xmm4, Simd_HalfPi	// xmm4 = +/- HALF_PI
		andps		xmm3, xmm4				// xmm3 = +/- HALF_PI or 0.0f
		movaps		xmm1, xmm0
		mulps		xmm1, xmm1				// xmm1 = s
		movaps		xmm2, Simd_atan_c0
		mulps		xmm2, xmm1
		addps		xmm2, Simd_atan_c1
		mulps		xmm2, xmm1
		addps		xmm2, Simd_atan_c2
		mulps		xmm2, xmm1
		addps		xmm2, Simd_atan_c3
		mulps		xmm2, xmm1
		addps		xmm2, Simd_atan_c4
		mulps		xmm2, xmm1
		addps		xmm2, Simd_atan_c5
		mulps		xmm2, xmm1
		addps		xmm2, Simd_atan_c6
		mulps		xmm2, xmm1
		addps		xmm2, Simd_atan_c7
		mulps		xmm2, xmm1
		addps		xmm2, Simd_one
		mulps		xmm2, xmm0
		addps		xmm2, xmm3
		movaps		[edx], xmm2					//45
	}


}
////////////////////////////////////////////////////////////////////////////////
THOR_INLINE float Asin( float f)
{
	using namespace SSE;
	// if abs(x)>0.5  asin(x)=pi/2-2asin(sqrt( (1-x)/2 ) )
	// asin(x)=xmm5 | ( xmm4&( pi/2- 2*asin)+ xmm3&( asin )
	__asm
	{
		movss	xmm0, f
		movss	xmm1, xmm0
		andps	xmm1, Simd_absMask		//xmm1=abs(x)
		movss	xmm5, xmm0
		andps	xmm5, Simd_signBitMask	//xmm5=signbit

		
		movss	xmm2, xmm1				//calc  xmm3=sqrt( (1-x)/2 )		
		movss	xmm3, Simd_one
		subss	xmm3, xmm2
		mulss	xmm3, Simd_half
		sqrtss	xmm3, xmm3
		minss   xmm1, xmm3			   //choose min
		movss	xmm0, xmm1			   //set min as argument
				
		cmpltps xmm2, Simd_half			//is abs(x) > 0.5 
		movss	xmm3, xmm2		 
		

		xorps	xmm2, Simd_not		
		movss	xmm4, xmm2			
		
		mulss	xmm1, xmm1
		movss   xmm2, Simd_asin_c9
		mulss	xmm2, xmm1
		addss	xmm2, Simd_asin_c8
		mulss	xmm2, xmm1
		addss	xmm2, Simd_asin_c7
		mulss	xmm2, xmm1
		addss	xmm2, Simd_asin_c6
		mulss	xmm2, xmm1
		addss	xmm2, Simd_asin_c5
		mulss	xmm2, xmm1
		addss	xmm2, Simd_asin_c4
		mulss	xmm2, xmm1
		addss	xmm2, Simd_asin_c3
		mulss	xmm2, xmm1
		addss	xmm2, Simd_asin_c2
		mulss	xmm2, xmm1
		addss	xmm2, Simd_asin_c1
		mulss	xmm2, xmm1
		addss	xmm2, Simd_asin_c0
		mulss	xmm2, xmm1
		addss   xmm2, Simd_one
		mulss	xmm2, xmm0		

		movss	xmm1, Simd_MinusTwo
		mulss	xmm1, xmm2
		addss	xmm1, Simd_HalfPi
		andps	xmm1, xmm4
		andps	xmm2, xmm3
		addss	xmm1, xmm2
		orps    xmm1, xmm5				//asin(-x)=-asin(x)
		
        movss	f, xmm1
		
	}

	return f;
}

THOR_INLINE void AsinvOp(const ThSseVec4& arg, ThSseVec4& res)
{
	using namespace SSE;
	// if abs(x)>0.5  asin(x)=pi/2-2asin(sqrt( (1-x)/2 ) )

	__asm
	{
		mov		edi, arg
		mov		esi, res
		movaps	xmm0, [edi]
		movaps	xmm1, xmm0
		andps	xmm1, Simd_absMask		//xmm1=abs(x)
		movaps	xmm5, xmm0
		andps	xmm5, Simd_signBitMask	//xmm5=signbit

		
		movaps	xmm2, xmm1				//calc  xmm3=sqrt( (1-x)/2 )		
		movaps	xmm3, Simd_one
		subps	xmm3, xmm2
		mulps	xmm3, Simd_half
		sqrtps	xmm3, xmm3
		minps   xmm1, xmm3				//choose min
		movaps	xmm0, xmm1				//set min as argument
				
		cmpltps xmm2, Simd_half			//is abs(x) > 0.5 
		movaps	xmm3, xmm2		 
	
		xorps	xmm2, Simd_not		
		movaps	xmm4, xmm2			
		
		mulps	xmm1, xmm1
		movaps   xmm2, Simd_asin_c9
		mulps	xmm2, xmm1
		addps	xmm2, Simd_asin_c8
		mulps	xmm2, xmm1
		addps	xmm2, Simd_asin_c7
		mulps	xmm2, xmm1
		addps	xmm2, Simd_asin_c6
		mulps	xmm2, xmm1
		addps	xmm2, Simd_asin_c5
		mulps	xmm2, xmm1
		addps	xmm2, Simd_asin_c4
		mulps	xmm2, xmm1
		addps	xmm2, Simd_asin_c3
		mulps	xmm2, xmm1
		addps	xmm2, Simd_asin_c2
		mulps	xmm2, xmm1
		addps	xmm2, Simd_asin_c1
		mulps	xmm2, xmm1
		addps	xmm2, Simd_asin_c0
		mulps	xmm2, xmm1
		addps   xmm2, Simd_one
		mulps	xmm2, xmm0		

		movaps	xmm1, Simd_MinusTwo
		mulps	xmm1, xmm2
		addps	xmm1, Simd_HalfPi
		andps	xmm1, xmm4
		andps	xmm2, xmm3
		addps	xmm1, xmm2
		orps    xmm1, xmm5				//asin(-x)=-asin(x)
		
        movaps	[esi], xmm1
		
	}	
}
////////////////////////////////////////////////////////////////////////////////
THOR_INLINE float Acos( float f)
{
	using namespace SSE;
	// acos(x) = pi/2 - asin(x)
	__asm
	{
		movss	xmm0, f
		movss	xmm1, xmm0
		andps	xmm1, Simd_absMask		//xmm1=abs(x)
		movss	xmm5, xmm0
		andps	xmm5, Simd_signBitMask	//xmm5=signbit

		
		movss	xmm2, xmm1				//calc  xmm3=sqrt( (1-x)/2 )		
		movss	xmm3, Simd_one
		subss	xmm3, xmm2
		mulss	xmm3, Simd_half
		sqrtss	xmm3, xmm3
		minss   xmm1, xmm3				//choose min
		movss	xmm0, xmm1				//set min as argument
				
		cmpltps xmm2, Simd_half			//is abs(x) > 0.5 
		movss	xmm3, xmm2		 
		

		xorps	xmm2, Simd_not		
		movss	xmm4, xmm2			
		
		
		mulss	xmm1, xmm1
		movss   xmm2, Simd_asin_c9
		mulss	xmm2, xmm1
		addss	xmm2, Simd_asin_c8
		mulss	xmm2, xmm1
		addss	xmm2, Simd_asin_c7
		mulss	xmm2, xmm1
		addss	xmm2, Simd_asin_c6
		mulss	xmm2, xmm1
		addss	xmm2, Simd_asin_c5
		mulss	xmm2, xmm1
		addss	xmm2, Simd_asin_c4
		mulss	xmm2, xmm1
		addss	xmm2, Simd_asin_c3
		mulss	xmm2, xmm1
		addss	xmm2, Simd_asin_c2
		mulss	xmm2, xmm1
		addss	xmm2, Simd_asin_c1
		mulss	xmm2, xmm1
		addss	xmm2, Simd_asin_c0
		mulss	xmm2, xmm1
		addss   xmm2, Simd_one
		mulss	xmm2, xmm0		

		movss	xmm1, Simd_MinusTwo
		mulss	xmm1, xmm2
		addss	xmm1, Simd_HalfPi
		andps	xmm1, xmm4
		andps	xmm2, xmm3
		addss	xmm1, xmm2
		orps    xmm1, xmm5				//asin(-x)=-asin(x)
		movss	xmm2, Simd_HalfPi
		subss   xmm2, xmm1
		
        movss	f, xmm2
		
	}

	return f;
}

THOR_INLINE void AcosvOp(const ThSseVec4& arg, ThSseVec4& res)
{
	using namespace SSE;
	// acos(x) = pi/2 - asin(x)
	
	__asm
	{
		mov		edi, arg
		mov		esi, res
		movaps	xmm0, [edi]
		movaps	xmm1, xmm0
		andps	xmm1, Simd_absMask
		movaps	xmm5, xmm0
		andps	xmm5, Simd_signBitMask

		
		movaps	xmm2, xmm1             //calc  sqrt( (1-x)/2 )		
		movaps	xmm3, Simd_one
		subps	xmm3, xmm2
		mulps	xmm3, Simd_half
		sqrtps	xmm3, xmm3
		minps   xmm1, xmm3
		movaps	xmm0, xmm1
				
		cmpltps xmm2, Simd_half		//is abs(x) > 0.5 
		movaps	xmm3, xmm2		 
	
		xorps	xmm2, Simd_not		
		movaps	xmm4, xmm2			
				
		mulps	xmm1, xmm1
		movaps  xmm2, Simd_asin_c9
		mulps	xmm2, xmm1
		addps	xmm2, Simd_asin_c8
		mulps	xmm2, xmm1
		addps	xmm2, Simd_asin_c7
		mulps	xmm2, xmm1
		addps	xmm2, Simd_asin_c6
		mulps	xmm2, xmm1
		addps	xmm2, Simd_asin_c5
		mulps	xmm2, xmm1
		addps	xmm2, Simd_asin_c4
		mulps	xmm2, xmm1
		addps	xmm2, Simd_asin_c3
		mulps	xmm2, xmm1
		addps	xmm2, Simd_asin_c2
		mulps	xmm2, xmm1
		addps	xmm2, Simd_asin_c1
		mulps	xmm2, xmm1
		addps	xmm2, Simd_asin_c0
		mulps	xmm2, xmm1
		addps   xmm2, Simd_one
		mulps	xmm2, xmm0		

		movaps	xmm1, Simd_MinusTwo
		mulps	xmm1, xmm2
		addps	xmm1, Simd_HalfPi
		andps	xmm1, xmm4		
		addps	xmm1, xmm2
		orps    xmm1, xmm5
		movaps	xmm2, Simd_HalfPi
		subps   xmm2, xmm1
		
        movaps	[esi], xmm2		
	}	
}


THOR_INLINE float Sign(float f)
{
	using namespace SSE;
	__asm
	{
		movss	xmm0, f
		andps	xmm0, Simd_signBitMask
		movaps	xmm1, Simd_one
		xorps	xmm1, xmm0

		movss	f, xmm1
	}

	return f;
}

THOR_INLINE void SignvOp(const ThSseVec4& arg, ThSseVec4& res)
{
	using namespace SSE;
	__asm
	{
		mov		edi, arg 
		mov		esi, res
		movaps	xmm0, [edi]
		andps	xmm0, Simd_signBitMask
		movaps	xmm1, Simd_one
		xorps	xmm1, xmm0

		movaps	[esi],xmm1
	}	
}


THOR_INLINE float Abs(float f)
{
	using namespace SSE;
	__asm
	{		
		movss	xmm0, f
		andps	xmm0, Simd_absMask

		movss	f, xmm0
	}
	return f;
}

THOR_INLINE void AbsvOp(const ThSseVec4& arg, ThSseVec4& res)
{
	using namespace SSE;
	__asm
	{
		mov		edi, arg
		mov     esi, res
		movaps	xmm0, [edi]
		andps	xmm0, Simd_absMask

		movaps	[esi], xmm0
	}
}


THOR_INLINE float Floor(float f)
{
	using namespace SSE;
	// x is between X.0 and X.999998 
	__asm
	{
		movss		xmm0, f
		subps		xmm0, Simd_half
		cvtps2dq	xmm0,xmm0
		cvtdq2ps	xmm0,xmm0
		
		movss		f, xmm0
	}

	return f;
}

THOR_INLINE void FloorvOp(const ThSseVec4& arg, ThSseVec4& res)
{
	using namespace SSE;
	__asm
	{
		mov			edi, arg
		mov			esi, res
		movaps		xmm0, [edi]
		subps		xmm0, Simd_half
		cvtps2dq	xmm0,xmm0
		cvtdq2ps	xmm0,xmm0

		movaps		[esi], xmm0
	}
}
////////////////////////////////////////////////////////////////////////////////
THOR_INLINE float Ceil(float f)
{// x is between X.000002 and X.999999 
	using namespace SSE;
	__asm
	{
		movss		xmm0, f
		addps		xmm0, Simd_half
		cvtps2dq	xmm0,xmm0
		cvtdq2ps	xmm0,xmm0
	
		movss		f, xmm0
	}
	return f;
}

THOR_INLINE void CeilvOp(const ThSseVec4& arg, ThSseVec4& res)
{
	using namespace SSE;
	// x is between X.000002 and X.999999
	__asm
	{
		mov			edi, arg
		mov			esi, res
		movaps		xmm0, [edi]
		addps		xmm0, Simd_half
		cvtps2dq	xmm0,xmm0
		cvtdq2ps	xmm0,xmm0

		movaps		[esi], xmm0
	}

}
////////////////////////////////////////////////////////////////////////////////
THOR_INLINE float Clamp(float f ,float min, float max)
{
	using namespace SSE;
	__asm
	{
		movss	xmm0, f
		movss	xmm1, min
		movss	xmm2, max
		maxps	xmm0, xmm1
		minps	xmm0, xmm2		

		movss	f,xmm0
	}

	return f;
}
////////////////////////////////////////////////////////////////////////////////
THOR_INLINE void ClampvOp(const ThSseVec4& arg , float min, float max, ThSseVec4& res)
{	
	using namespace SSE;
	__asm
	{
		mov		edi, arg
		mov		esi, res
		movaps	xmm0, [edi]
		movss	xmm1, min
		movss	xmm2, max
		shufps	xmm1, xmm1, 0
		shufps	xmm2, xmm2, 0
		maxps	xmm0, xmm1
		minps	xmm0, xmm2		

		movaps	[esi],xmm0
	}
}
////////////////////////////////////////////////////////////////////////////////
THOR_INLINE float  Pwr2(int p)
{
//computes 2^p by moving p into exponent bits of a float
	float pw;	
	using namespace SSE;
    __asm
	{
		movd		xmm0, p			
		paddd		xmm0, Simd_IEEE_BIASt	//bias exponent
		pslld		xmm0, 23				//move bits to an exponent part of a float
				
		movss		pw, xmm0
	}


	return pw;
}
////////////////////////////////////////////////////////////////////////////////
THOR_INLINE void Pwr2vOp(ThSseVec4& arg, const ThSseVec4i& pwr)
{
	using namespace SSE;
//computes 2^p by moving p into exponent bits of a float
    __asm
	{
		mov			edi, arg
		mov			esi, pwr

		movaps		xmm0, [esi]					
		paddd		xmm0, Simd_IEEE_BIASt	//bias exponent
		pslld		xmm0, 23				//move bits to an exponent part of a float
				
		movaps		[edi], xmm0
	}	
}
////////////////////////////////////////////////////////////////////////////////
THOR_INLINE void  MantissaExp(float f, float &m, float &e, float &p )
{
//extracts	mantissa and exponent from f and computes 2^e
	using namespace SSE;
	__asm
	{
		mov			edi, m
		mov			ecx, e
		mov			esi, p

		movss		xmm0, f		
		movss		xmm1, xmm0
		orps		xmm0, Simd_IEEE_FMNTMASKt		//extract	mantissa  it has exponent 01111111 if f is normalized
		andps		xmm0, Simd_IEEE_FMNTMASKt1		//remove highest bit from mantissa   e=0..127 => f<=1 e>127 => f>1
		andps		xmm1, Simd_IEEE_FEXPMASKt		//extract  exponent 

		movss		xmm2, xmm1						//xmm2=2^exp

		psrld		xmm1, 23
		psubd		xmm1, Simd_IEEE_BIASt			//unbias exponent e=e-127
		movss		xmm3, xmm1									
		
		cvtdq2ps	xmm3, xmm3

		movss		[edi], xmm0						//return mantissa
		movss		[ecx], xmm3						//return exponent
		movss		[esi], xmm2						//return 2^e
	}

}

////////////////////////////////////////////////////////////////////////////////
THOR_INLINE float Exp(float f)
{
	using namespace SSE;
	// e^x=2^(x/ln2)=2^(k.f)=2^k*2^f=2^k*e^(f*ln2)		

	__asm
	{
		movss		xmm0, f					
		

		mulss		xmm0, Simd_1divLn2		//xmm0=x/ln2
		movss		xmm1, xmm0				//xmm1=xmm0
		subss		xmm1, Simd_half		//round down

		cvtps2dq	xmm1, xmm1				//xmm1=k

		movss		xmm2, xmm1				//compute 2^k
		movss		xmm3, xmm2		
		paddd		xmm2, Simd_IEEE_BIASt	//unbias exponent
		pslld		xmm2, 23				//move bits to an exponent part of a float				

		cvtdq2ps	xmm1, xmm1				//xmm1=k		
			
		subss		xmm0, xmm1				//xmm2=f		
		mulss		xmm0, Simd_Ln2			//calc f*ln2
	
		movss		xmm4, Simd_exp_c6
		mulss		xmm4, xmm0
		addss		xmm4, Simd_exp_c5
		mulss		xmm4, xmm0
		addss		xmm4, Simd_exp_c4
		mulss		xmm4, xmm0
		addss		xmm4, Simd_exp_c3
		mulss		xmm4, xmm0
		addss		xmm4, Simd_exp_c2
		mulss		xmm4, xmm0
		addss		xmm4, Simd_exp_c1
		mulss		xmm4, xmm0
		addss		xmm4, Simd_exp_c0
		mulss		xmm4, xmm0
		addss		xmm4, Simd_one
		mulss		xmm4, xmm0
		addss		xmm4, Simd_one

		mulss		xmm4, xmm2
		
		movss		f, xmm4		
		
	}
	
	return f;
}


THOR_INLINE void ExpvOp(const ThSseVec4& arg, ThSseVec4& res)
{
	using namespace SSE;
	// e^x=2^(x/ln2)=2^(k.f)=2^k*2^f=2^k*e^(f*ln2)
	

	__asm
	{
		mov			edi, arg
		mov			esi, res
		movaps		xmm0, [edi]					
		

		mulps		xmm0, Simd_1divLn2		//xmm0=x/ln2
		movaps		xmm1, xmm0				//xmm1=xmm0
		subps		xmm1, Simd_half			//round down

		cvtps2dq	xmm1, xmm1				//xmm1=k

		movaps		xmm2, xmm1				//compute 2^k
		movaps		xmm3, xmm2		
		paddd		xmm2, Simd_IEEE_BIASt	//unbias exponent
		pslld		xmm2, 23				//move bits to an exponent part of a float
				

		cvtdq2ps	xmm1, xmm1				//xmm1=k		
			
		subps		xmm0, xmm1				//xmm2=f		
		mulps		xmm0, Simd_Ln2			//calc f*ln2
	
		movaps		xmm4, Simd_exp_c6
		mulps		xmm4, xmm0
		addps		xmm4, Simd_exp_c5
		mulps		xmm4, xmm0
		addps		xmm4, Simd_exp_c4
		mulps		xmm4, xmm0
		addps		xmm4, Simd_exp_c3
		mulps		xmm4, xmm0
		addps		xmm4, Simd_exp_c2
		mulps		xmm4, xmm0
		addps		xmm4, Simd_exp_c1
		mulps		xmm4, xmm0
		addps		xmm4, Simd_exp_c0
		mulps		xmm4, xmm0
		addps		xmm4, Simd_one
		mulps		xmm4, xmm0
		addps		xmm4, Simd_one

		mulps		xmm4, xmm2
		
		movaps		[esi], xmm4		
		
	}
}
////////////////////////////////////////////////////////////////////////////////
THOR_INLINE float Ln(float f)
{
	using namespace SSE;
	//ln(x)=ln(m*2^exp)=ln(m)+exp*ln 2
	//t=(m-1)/(m+1);
	//float t2=t*t;
	//return 2*t*(1.0+t2*(1/3.0+t2*(1/5.0+1/7.0*t2)))+e*log(2.0f);

	__asm
	{
		
		movss		xmm0, f		
		movss		xmm1, xmm0
		orps		xmm0, Simd_IEEE_FMNTMASKt		//extract	mantissa
		andps		xmm0, Simd_IEEE_FMNTMASKt1		//remove highest bit from mantissa   e=0..127 => f<=1 e>127 => f>1
		andps		xmm1, Simd_IEEE_FEXPMASKt		//extract   exponent

		psrld		xmm1, 23
		psubd		xmm1, Simd_IEEE_BIASt			//unbias exponent e=e-127
		
		cvtdq2ps	xmm1, xmm1						//xmm1=exp

		movss		xmm2, xmm0						//xmm2=m
		subss		xmm0, Simd_one					//xmm0=m-1
		addss		xmm2, Simd_one					//xmm2=m+1
		//rcpss		xmm2, xmm2						//xmm2=1/(m+1)
		divss		xmm0, xmm2						//xmm0=t=(m-1)/(m+1)
		mulss		xmm1, Simd_Ln2					//xmm1=exp*ln2

		movss		xmm2, xmm0
		mulss		xmm2, xmm2						//xmm2=t*t

		movss		xmm3, Simd_ln_c3				
		mulss		xmm3, xmm2						//xmm3=1/7*t2
		addss		xmm3, Simd_ln_c2
		mulss		xmm3, xmm2						//xmm3=t2*(1/5+1/7*t2)
		addss		xmm3, Simd_ln_c1
		mulss		xmm3, xmm2						//xmm3=t2*(1/3+t2*(1/5+1/7*t2))
		addss		xmm3, Simd_ln_c0
		mulss		xmm3, xmm0						//xmm3=t*(1+t2*(1/3+t2*(1/5+1/7*t2)))

		addss		xmm3, xmm3						//xmm3=2*t*(1+t2*(1/3+t2*(1/5+1/7*t2)))

		addss		xmm3, xmm1

		movss		f, xmm3
	}

	return f;
}

THOR_INLINE void LnvOp(const ThSseVec4& arg, ThSseVec4& res)
{
	using namespace SSE;
	//ln(x)=ln(m*2^exp)=ln(m)+exp*ln 2
	//t=(m-1)/(m+1);
	//float t2=t*t;
	//return 2*t*(1.0+t2*(1/3.0+t2*(1/5.0+1/7.0*t2)))+e*log(2.0f);
	

	__asm
	{
		mov			edi, arg
		mov			esi, res		
		movaps		xmm0, [edi]		
		movaps		xmm1, xmm0
		orps		xmm0, Simd_IEEE_FMNTMASKt		//extract	mantissa
		andps		xmm0, Simd_IEEE_FMNTMASKt1		//remove highest bit from mantissa   e=0..127 => f<=1 e>127 => f>1
		andps		xmm1, Simd_IEEE_FEXPMASKt		//extract   exponent

		psrld		xmm1, 23
		psubd		xmm1, Simd_IEEE_BIASt			//unbias exponent e=e-127
		
		cvtdq2ps	xmm1, xmm1						//xmm1=exp

		movaps		xmm2, xmm0						//xmm2=m
		subps		xmm0, Simd_one					//xmm0=m-1
		addps		xmm2, Simd_one					//xmm2=m+1
		//rcpss		xmm2, xmm2						//xmm2=1/(m+1)
		divps		xmm0, xmm2						//xmm0=t=(m-1)/(m+1)
		mulps		xmm1, Simd_Ln2					//xmm1=exp*ln2

		movaps		xmm2, xmm0
		mulps		xmm2, xmm2						//xmm2=t*t

		movaps		xmm3, Simd_ln_c3				
		mulps		xmm3, xmm2						//xmm3=1/7*t2
		addps		xmm3, Simd_ln_c2
		mulps		xmm3, xmm2						//xmm3=t2*(1/5+1/7*t2)
		addps		xmm3, Simd_ln_c1
		mulps		xmm3, xmm2						//xmm3=t2*(1/3+t2*(1/5+1/7*t2))
		addps		xmm3, Simd_ln_c0
		mulps		xmm3, xmm0						//xmm3=t*(1+t2*(1/3+t2*(1/5+1/7*t2)))

		addps		xmm3, xmm3						//xmm3=2*t*(1+t2*(1/3+t2*(1/5+1/7*t2)))

		addps		xmm3, xmm1

		movaps		[esi], xmm3
	}

}

}}