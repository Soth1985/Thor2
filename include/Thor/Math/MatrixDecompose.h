#pragma once

#include <Thor/Math/FixedMatrixFwd.h>
#include <Thor/Math/FixedVectorFwd.h>
#include <Thor/Math/ThorMathExport.h>

namespace Thor{
	THOR_MATH_DLL float PolarDecomposition(const ThMat4x4& m, ThQuat& q, ThMat4x4& s);
}