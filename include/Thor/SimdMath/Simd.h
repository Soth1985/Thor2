#pragma once

#include <Thor/Core/Common.h>

#ifdef THOR_PLATFORM_APPLE
    #include <simd/simd.h>
#else
    #include "Simd/vector.h"
    #include "Simd/matrix.h"
    #include "Simd/quaternion.h"
#endif
