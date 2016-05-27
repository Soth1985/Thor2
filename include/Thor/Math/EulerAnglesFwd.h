#pragma once

#include <Thor/Core/Common.h>
#include <Thor/Math/Allocators.h>

namespace Thor{

template <class RealT,class AllocatorT = ThStandartAllocator>
struct ThEulerAngles;

typedef ThEulerAngles<float> ThAngles;

}//Thor