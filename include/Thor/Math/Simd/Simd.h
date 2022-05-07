#pragma once

#include <Thor/Core/Common.h>

#ifdef THOR_PLATFORM_APPLE
    #include <simd/simd.h>
#else
    #include "vector.h"
    #include "matrix.h"
    #include "quaternion.h"
#endif
