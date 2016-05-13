#pragma once

#include <Thor/Framework/Common.h>
#include <Thor/ThorMath/Allocators.h>

namespace Thor{

template <class RealT,class AllocatorT = ThStandartAllocator>
struct ThEulerAngles;

typedef ThEulerAngles<float> ThAngles;

}//Thor