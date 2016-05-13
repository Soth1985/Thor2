#pragma once

#include <Thor/ThorMath/FixedMatrix.h>
#include <Thor/ThorMath/FixedVector.h>
#include <Thor/ThorMath/Quaternion.h>
#include <Thor/ThorMath/ThorMathExport.h>

namespace Thor{
	THOR_MATH_DLL float PolarDecomposition(const ThMat4x4& m, ThQuat& q, ThMat4x4& s);
}