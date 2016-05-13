#pragma once

#include <Thor/Framework/Common.h>
#include <Thor/ThorMath/Allocators.h>

namespace Thor{

struct generic_vec_tag{};
struct quaternion_tag{};
struct sse4f_vec_tag{};

template < 
			class DataT, unsigned int sz, //sz is the number of elements in the vector
			class TagT = generic_vec_tag,
			class AllocatorT = ThStandartAllocator
		 >
class ThFixedVector;

typedef ThFixedVector< ThF32,4,sse4f_vec_tag,ThAlignedAllocator<> > ThSseVec4;
typedef ThFixedVector< ThI32,4,generic_vec_tag,ThStandartAllocator > ThSseVec4i;

typedef ThFixedVector< ThF32,4 > ThVec4;
typedef ThFixedVector< ThF32,3 > ThVec3;
typedef ThFixedVector< ThF32,2 > ThVec2;
typedef ThFixedVector< ThF32,4,quaternion_tag > ThQuat;

typedef ThFixedVector< ThI32,4 > ThVec4i;
typedef ThFixedVector< ThI32,3 > ThVec3i;
typedef ThFixedVector< ThI32,2 > ThVec2i;

typedef ThFixedVector< ThU32,4 > ThVec4ui;
typedef ThFixedVector< ThU32,3 > ThVec3ui;
typedef ThFixedVector< ThU32,2 > ThVec2ui;

typedef ThFixedVector< ThI16,4 > ThVec4s;
typedef ThFixedVector< ThI16,3 > ThVec3s;
typedef ThFixedVector< ThI16,2 > ThVec2s;

typedef ThFixedVector< ThU16,4 > ThVec4us;
typedef ThFixedVector< ThU16,3 > ThVec3us;
typedef ThFixedVector< ThU16,2 > ThVec2us;

typedef ThFixedVector< ThF64,4 > ThVec4d;
typedef ThFixedVector< ThF64,3 > ThVec3d;
typedef ThFixedVector< ThF64,2 > ThVec2d;
typedef ThFixedVector< ThF64,4,quaternion_tag > ThQuatd;

typedef ThFixedVector< ThBool,4 > ThVec4b;
typedef ThFixedVector< ThBool,3 > ThVec3b;
typedef ThFixedVector< ThBool,2 > ThVec2b;

}//Thor