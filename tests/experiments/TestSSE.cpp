#include <Thor/Math/Math.h>
#include <type_traits>

using namespace Thor;
using namespace Thor::Math;

void TestSSE()
{
	{
		float Res = Sqrt(4.0f);
		printf("%.3f", Res);
	}

	{
		ThSseVec4 Vec(4.0f, 9.0f, 16.0f, 25.0f);
		ThSseVec4 Res;
		SqrtvOp(Vec, Res);
		printf("%.3f", Res(1));
	}
	
	{
		float Res = Sin(7689.0f);
		printf("%.3f", Res);
	}

	{
		ThSseVec4 Arg(100.0f, 200.0f, -300.0f, -400.f);
		ThSseVec4 Res;
		SinvOp(Arg, Res);
		printf("%.3f", Res(1));
	}

	{
		float Res = Cos(7689.0f);
		printf("%.3f", Res);
	}

	{
		ThSseVec4 Arg(100.0f, 200.0f, -300.0f, -400.f);
		ThSseVec4 Res;
		CosvOp(Arg, Res);
		printf("%.3f", Res(1));
	}

	{
		float ResS, ResC;
		SinCos(7689.0, ResS, ResC);
		printf("%.3f", ResS);
	}

	{
		ThSseVec4 Arg(100.0f, 200.0f, -300.0f, -400.f);
		ThSseVec4 ResS, ResC;
		SinCosvOp(Arg, ResS, ResC);
		printf("%.3f", ResS(1));
	}

	{
		float Res = Atan(-2.20f);
		printf("%.3f", Res);
	}

	{
		ThSseVec4 Arg(0.1f, 0.2f, -0.3f, -400.f);
		ThSseVec4 Res;
		AtanvOp(Arg, Res);
		printf("%.3f", Res(1));
	}

	{
		float Res = Atan2(66.0f, -30.3f);
		float Z0 = Atan2(0.0f, 0.0f);
		float Z1 = Atan2(1.0f, 0.0f);
		float Z2 = Atan2(-1.0f, 0.0f);
		float R1 = ::atan2f(66.0f, -30.3f);
		printf("%.3f", Res);
	}

	{
		ThSseVec4 X(10.1f, -20.2f, -30.3f, 400.f);
		ThSseVec4 Y(3.0f, 7.0f, -66.0f, -99.0f);
		ThSseVec4 Res, Res1;
		Atan2vOp(Y, X, Res);

		for (int i = 0; i < 4; ++i)
		{
			Res1(i) = ::atan2f(Y(i), X(i));
		}

		printf("%.3f", Res(1));
	}

	{
		float Res = Asin(0.72f);
		float Res1 = ::asinf(0.72f);
		printf("%.3f", Res);
	}

	{
		ThSseVec4 Arg(0.1f, 0.2f, -3.3f, 1.5f);
		ThSseVec4 Res,Res1;
		AsinvOp(Arg, Res);

		for (int i = 0; i < 4; ++i)
		{
			Res1(i) = ::asinf(Arg(i));
		}

		printf("%.3f", Res(1));
	}

	{
		float Res = Acos(0.72f);
		float Res1 = ::acosf(0.72f);
		printf("%.3f", Res);
	}

	{
		ThSseVec4 Arg(0.1f, 0.2f, -3.3f, 1.5f);
		ThSseVec4 Res,Res1;
		AcosvOp(Arg, Res);
		for (int i = 0; i < 4; ++i)
		{
			Res1(i) = ::acosf(Arg(i));
		}
		printf("%.3f", Res(1));
	}

	{
		float Res = Sign(-0.72f);
		printf("%.3f", Res);
	}

	{
		ThSseVec4 Arg(0.1f, 0.0f, -0.0f, -1.0f);
		ThSseVec4 Res;
		SignvOp(Arg, Res);
		printf("%.3f", Res(1));
	}

	{
		float Res = Abs(-0.72f);
		float Res1 = ::fabs(-0.72f);
		printf("%.3f", Res);
	}

	{
		ThSseVec4 Arg(0.1f, 0.0f, -0.0f, -1.0f);
		ThSseVec4 Res, Res1;
		AbsvOp(Arg, Res);
		for (int i = 0; i < 4; ++i)
		{
			Res1(i) = ::fabs(Arg(i));
		}
		printf("%.3f", Res(1));
	}
	
	{
		float Res = Floor(-0.72f);
		printf("%.3f", Res);
	}

	{
		ThSseVec4 Arg(1.0f, 12.45f, -0.0f, -1.0f);
		ThSseVec4 Res;
		FloorvOp(Arg, Res);
		printf("%.3f", Res(1));
	}

	{
		float Res = Ceil(-0.72f);
		printf("%.3f", Res);
	}

	{
		ThSseVec4 Arg(1.0f, 12.45f, -0.0f, -1.0f);
		ThSseVec4 Res;
		CeilvOp(Arg, Res);
		printf("%.3f", Res(1));
	}

	{
		float Res = Clamp(-0.72f, -1.0f, 1.0f);
		printf("%.3f", Res);
	}

	{
		ThSseVec4 Arg(1.0f, 12.45f, -0.0f, -1.0f);
		ThSseVec4 Res;
		ClampvOp(Arg, -1.0f, 1.0f, Res);
		printf("%.3f", Res(1));
	}

	{
		float Res = Pwr2(4);
		printf("%.3f", Res);
	}

	{
		ThSseVec4i Pwr(1, 2, 3, -5);
		ThSseVec4 Res;
		Pwr2vOp(Pwr, Res);
		printf("%.3f", Res(1));
	}

	{
		float M, E, P;
		MantissaExp(34223.12367f, M, E, P);
		printf("%.3f", M);
	}

	{
		float Arg = 2.789f;
		float Res = Exp(Arg);
		float Res1 = ::expf(Arg);
		printf("%.3f", Res);
	}

	{
		ThSseVec4 Arg(-8.90f, 2.12f, -0.7f, 0.33f);
		ThSseVec4 Res,Res1;
		ExpvOp(Arg, Res);

		for (int i = 0; i < 4; ++i)
		{
			Res1(i) = ::expf(Arg(i));
		}

		printf("%.3f", Res(1));
	}

	{
		float Arg = 2.789f;
		float Res = Ln(Arg);
		float Res1 = ::logf(Arg);
		printf("%.3f", Res);
	}

	{
		ThSseVec4 Arg(-8.90f, 2.12f, -0.7f, 0.33f);
		ThSseVec4 Res, Res1;
		LnvOp(Arg, Res);

		for (int i = 0; i < 4; ++i)
		{
			Res1(i) = ::logf(Arg(i));
		}

		printf("%.3f", Res(1));
	}
}

int main()
{
    bool pod = std::is_pod<ThVec3f>::value;
    bool trivial = std::is_trivial<ThVec3f>::value;
    bool stl = std::is_standard_layout<ThVec3f>::value;
    bool tvc = std::is_trivially_copyable<ThVec3f>::value;
    bool tdc = std::is_trivially_default_constructible<ThVec3f>::value;
	TestSSE();
	return 0;
}