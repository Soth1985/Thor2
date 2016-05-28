#include <Thor/Math/MatrixDecompose.h>
#include <Thor/Math/FixedVector.h>
#include <Thor/Math/FixedMatrix.h>
#include <Thor/Math/Quaternion.h>

extern "C"
{
	#include <Thor/Math/Decompose.h>
}

namespace Thor{
	float PolarDecomposition(const ThMat4x4& mat, ThQuat& q, ThMat4x4& s)
	{
		HMatrix M,Q,S;
		ThMat4x4 m(mat);
		m.Transpose();
		memcpy(&M[0][0], &m(0,0), 16 * sizeof(float));
		float res = polar_decomp(M,Q,S);
		QuatXX rot = Qt_FromMatrix(Q);
		q.x() = rot.x;
		q.y() = rot.y;
		q.z() = rot.z;
		q.w() = rot.w;
		memcpy(&s(0,0), &S[0][0], 16 * sizeof(float));
		s.Transpose();
		return res;
	}
}