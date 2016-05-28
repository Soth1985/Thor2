#pragma once

#include <Thor/Core/Common.h>
#include <Thor/Math/FixedVector.h>
#include <Thor/Math/FixedMatrix.h>
#include <Thor/Math/EulerAngles.h>
#include <Thor/Math/IntersectionTests.h>
#include <Thor/Math/ThorMath.h>

#ifdef THOR_ENABLE_SSE_MATH
    #include <Thor/Math/MathSSE.h>
#endif