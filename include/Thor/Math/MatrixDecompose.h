#pragma once

#include <Thor/Math/FixedMatrix.h>
#include <Thor/Math/FixedVector.h>
#include <Thor/Math/Quaternion.h>
#include <Thor/Math/ThorMathExport.h>

namespace Thor{
	THOR_MATH_DLL float PolarDecomposition(const ThMat4x4& m, ThQuat& q, ThMat4x4& s);
}