#pragma once

#include <Thor/Core/Common.h>

namespace Thor{

struct generic_mat_tag{};
struct sse4x4f_mat_tag{};

template < 
			class DataT, unsigned int rows, unsigned int columns, //rows x columns matrix
			class TagT = generic_mat_tag
		 >
class ThFixedMatrix;

typedef ThFixedMatrix<float,4,4> ThMat4x4f;
typedef ThFixedMatrix<float,3,3> ThMat3x3f;
typedef ThFixedMatrix<float,2,2> ThMat2x2f;
typedef ThFixedMatrix<float,2,3> ThMat2x3f;
typedef ThFixedMatrix<float,2,4> ThMat2x4f;
typedef ThFixedMatrix<float,3,2> ThMat3x2f;
typedef ThFixedMatrix<float,3,4> ThMat3x4f;
typedef ThFixedMatrix<float,4,2> ThMat4x2f;
typedef ThFixedMatrix<float,4,3> ThMat4x3f;

typedef ThFixedMatrix<double,4,4> ThMat4x4d;
typedef ThFixedMatrix<double,3,3> ThMat3x3d;
typedef ThFixedMatrix<double,2,2> ThMat2x2d;
typedef ThFixedMatrix<double,2,3> ThMat2x3d;
typedef ThFixedMatrix<double,2,4> ThMat2x4d;
typedef ThFixedMatrix<double,3,2> ThMat3x2d;
typedef ThFixedMatrix<double,3,4> ThMat3x4d;
typedef ThFixedMatrix<double,4,2> ThMat4x2d;
typedef ThFixedMatrix<double,4,3> ThMat4x3d;

}//Thor