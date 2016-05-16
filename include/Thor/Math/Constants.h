#pragma once

namespace Thor{
	namespace Math{

	static float CreatePositiveInfinity()
	{
		int Mask = 0x7F800000;
		return reinterpret_cast<float&>(Mask);
	}

	static float CreateNegativeInfinity()
	{
		int Mask = 0xFF800000;
		return reinterpret_cast<float&>(Mask);
	}

	static float CreateNaN()
	{
		int Mask = 0x7FFFFFFF;
		return reinterpret_cast<float&>(Mask);
	}

	const float Pi = 3.1415926535f;//3.14159265358979323846 msdn math::pi				   
	const float NearZero = 3e-38f;
	const float E4 = 1e-4f;
	const float HalfPi = 1.570796327f; 
	const float TwoPi = 6.283185306f; 
	const float PositiveInfinity = CreatePositiveInfinity();
	const float NegativeInfinity = CreateNegativeInfinity();
	const float NaN = CreateNaN();
		
		namespace SSE{

			#define ALIGN16(x) __declspec(align(16)) x

#ifdef THOR_X64
			#include <xmmintrin.h>
			#define ALIGN4_INIT1F( X, INIT )			static __m128 X = _mm_set_ps((INIT), (INIT), (INIT), (INIT))
			#define ALIGN4_INIT4F( X, I0, I1, I2, I3 )	static __m128 X = _mm_set_ps((I0), (I1), (I2), (I3))
			#define ALIGN4_INIT1I( X, INIT )			static __m128 X = _mm_castsi128_ps(_mm_set_epi32 ((INIT), (INIT), (INIT), (INIT)))
			#define ALIGN4_INIT4I( X, I0, I1, I2, I3 )	static __m128 X =  _mm_castsi128_ps(_mm_set_epi32 ((I0), (I1), (I2), (I3)))
#else
			#define ALIGN4_INIT1F( X, INIT )			ALIGN16( static float X[4] ) = { (INIT), (INIT), (INIT), (INIT) }
			#define ALIGN4_INIT4F( X, I0, I1, I2, I3 )	ALIGN16( static float X[4] ) = { (I0), (I1), (I2), (I3) }
			#define ALIGN4_INIT1F( X, INIT )			ALIGN16( static ThI32 X[4] ) = { (INIT), (INIT), (INIT), (INIT) }
			#define ALIGN4_INIT4F( X, I0, I1, I2, I3 )	ALIGN16( static ThI32 X[4] ) = { (I0), (I1), (I2), (I3) }
#endif

			#define SHUFFLE(z, y, x, w)   ( (z<<6) | (y<<4) | (x<<2) | w  )

			ALIGN4_INIT1F(Simd_zero,     0.0f          );
			ALIGN4_INIT1F(Simd_one,      1.0f          );
			ALIGN4_INIT1F(Simd_MinusOne,-1.0f          );
			ALIGN4_INIT1F(Simd_two,      2.0f          );
			ALIGN4_INIT1F(Simd_MinusTwo,-2.0f          );
			ALIGN4_INIT1F(Simd_four,     4.0f          );
			ALIGN4_INIT1F(Simd_half,     0.5f          );
			ALIGN4_INIT1F(Simd_Pi,       Pi            );
			ALIGN4_INIT1F(Simd_MinPi, -Pi);
			ALIGN4_INIT1F(Simd_2Pi,      TwoPi        );
			ALIGN4_INIT1F(Simd_HalfPi,   HalfPi       );
			ALIGN4_INIT1F(Simd_MinHalfPi,-HalfPi      );
			ALIGN4_INIT1F(Simd_1div2Pi,  1.0f / (2.0f * Pi));
			ALIGN4_INIT1F(Simd_0499,     0.4999999f );
			ALIGN4_INIT1F(Simd_1divLn2,  1.442695f		);
			ALIGN4_INIT1F(Simd_Ln2,      0.69314718f	);
			ALIGN4_INIT1F(Simd_MaxExp,   3.399969e38f	);
			ALIGN4_INIT1F(Simd_MaxExpArg,88.722f		);

			ALIGN4_INIT1F(Simd_SinC0, -1.666667e-01f);
			ALIGN4_INIT1F(Simd_SinC1,  8.333333e-03f);
			ALIGN4_INIT1F(Simd_SinC2, -1.984127e-04f);
			ALIGN4_INIT1F(Simd_SinC3,  2.755732e-06f);
			ALIGN4_INIT1F(Simd_SinC4, -2.505211e-08f);

			ALIGN4_INIT1F(Simd_CosC0, -0.500000f    );
			ALIGN4_INIT1F(Simd_CosC1,  4.166667e-02f);
			ALIGN4_INIT1F(Simd_CosC2, -1.388889e-03f);
			ALIGN4_INIT1F(Simd_CosC3,  2.480159e-05f);
			ALIGN4_INIT1F(Simd_CosC4, -2.755732e-07f);

			ALIGN4_INIT1F(Simd_Atan, 0.28f);

			ALIGN4_INIT1F( Simd_atan_c0,  0.0028662257f );
			ALIGN4_INIT1F( Simd_atan_c1, -0.0161657367f );
			ALIGN4_INIT1F( Simd_atan_c2,  0.0429096138f );
			ALIGN4_INIT1F( Simd_atan_c3, -0.0752896400f );
			ALIGN4_INIT1F( Simd_atan_c4,  0.1065626393f );
			ALIGN4_INIT1F( Simd_atan_c5, -0.1420889944f );
			ALIGN4_INIT1F( Simd_atan_c6,  0.1999355085f );
			ALIGN4_INIT1F( Simd_atan_c7, -0.3333314528f );

			ALIGN4_INIT1F( Simd_asin_c0,  0.1666666667f );
			ALIGN4_INIT1F( Simd_asin_c1,  0.0750000000f );
			ALIGN4_INIT1F( Simd_asin_c2,  0.0446428570f );
			ALIGN4_INIT1F( Simd_asin_c3,  0.0303819440f );
			ALIGN4_INIT1F( Simd_asin_c4,  0.0223721590f );
			ALIGN4_INIT1F( Simd_asin_c5,  0.0173527640f );
			ALIGN4_INIT1F( Simd_asin_c6,  0.0139648430f );
			ALIGN4_INIT1F( Simd_asin_c7,  0.0115518000f );
			ALIGN4_INIT1F( Simd_asin_c8,  0.0097616095f );
			ALIGN4_INIT1F( Simd_asin_c9,  0.0083903358f );

			ALIGN4_INIT1F( Simd_exp_c0,   0.5000000000f );
			ALIGN4_INIT1F( Simd_exp_c1,   0.1666666667f );
			ALIGN4_INIT1F( Simd_exp_c2,   0.0416666667f );
			ALIGN4_INIT1F( Simd_exp_c3,   0.0083333333f );
			ALIGN4_INIT1F( Simd_exp_c4,   0.0013888889f );
			ALIGN4_INIT1F( Simd_exp_c5,   0.0001984127f );
			ALIGN4_INIT1F( Simd_exp_c6,   0.0000248016f );

			ALIGN4_INIT1F( Simd_ln_c0,    1.0000000000f );
			ALIGN4_INIT1F( Simd_ln_c1,    0.3333333333f );
			ALIGN4_INIT1F( Simd_ln_c2,    0.2000000000f );
			ALIGN4_INIT1F( Simd_ln_c3,    0.1428571420f );

			ALIGN4_INIT1I( Simd_64,   64 );
			ALIGN4_INIT1F( Simd_Temp,   0.0f );
			ALIGN4_INIT1F( Simd_NaN, NaN);

			ALIGN4_INIT1I( Simd_signBitMask, (ThI32) ( 1 << 31 ) );
			ALIGN4_INIT1I( Simd_absMask, (ThI32) ~( 1 << 31 ) );
			ALIGN4_INIT1I( Simd_not, (ThI32)0xFFFFFFFF );
			ALIGN4_INIT1I( Simd_LeastBit, (ThI32)0x00000001 );

			ALIGN4_INIT1I( Simd_IEEE_FEXPMASKt,(ThI32)0x7F800000 );//01111111100000000000000000000000 

			ALIGN4_INIT1I(Simd_IEEE_FMNTMASKt, (ThI32) 0x3F800000 );//00111111100000000000000000000000
			ALIGN4_INIT1I(Simd_IEEE_FMNTMASKt1, (ThI32)0xBFFFFFFF );//10111111111111111111111111111111

			ALIGN4_INIT1I(Simd_IEEE_BIASt,  127 );

			ALIGN4_INIT4F( Simd_0001, 0.0f, 0.0f, 0.0f, 1.0f );
			ALIGN4_INIT4F( Simd_0010, 0.0f, 0.0f, 1.0f, 0.0f );
			ALIGN4_INIT4F( Simd_0100, 0.0f, 1.0f, 0.0f, 0.0f );
			ALIGN4_INIT4F( Simd_1000, 1.0f, 0.0f, 0.0f, 0.0f );
			ALIGN4_INIT4F( Simd_1001, 1.0f, 0.0f, 0.0f, 1.0f );

			ALIGN4_INIT4I(Simd_ZeroW, (ThI32)0xFFFFFFFF, (ThI32)0xFFFFFFFF, (ThI32)0xFFFFFFFF, (ThI32)0x00000000);
			};//SSE

	};//Math
};//Thor