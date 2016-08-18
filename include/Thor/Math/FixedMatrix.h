#pragma once

#include <Thor/Math/ListConstructor.h>
#ifdef USE_MATRIX_EXPRESSION_TEMPLATES
	#include <Thor/Math/OpTags.h>
	#include <Thor/Math/ExpressionNodes.h>
#endif

#include <Thor/Math/FixedMatrixFwd.h>
#include <Thor/Math/FixedVectorFwd.h>
#include <Thor/Math/ThorMath.h>

//#define ROW_MAJOR_MATRIX

#ifndef ROW_MAJOR_MATRIX
	#define COLUMN_MAJOR_MATRIX
#endif


namespace Thor{

#include <Thor/Math/FixedMatrixGeneric.inl>
//#include <FixedMatrixGeneric.inl>

#pragma region Generic matrix specialization

template < 
			class DataT, unsigned int rows, unsigned int columns,
			class TagT
		 >
class ThFixedMatContainer
{
public:

	ThFixedMatContainer()
	{
		memset(&Mat[0], 0, rows * columns * sizeof(DataT) );
	};
	
	ThFixedMatContainer( const ThFixedMatContainer& copy )
	{
		memcpy(&Mat[0], &copy.Mat[0], rows * columns * sizeof(DataT) );
	};
#ifdef USE_MATRIX_EXPRESSION_TEMPLATES
	template < class ExprT >
	THOR_INLINE void assign_expr( const ExprT& e )
	{
		ThFixedMatContainer tmp;
	
		for( unsigned int i = 0; i < rows * columns; ++i )
			tmp.Mat[i] = e.tree.at<DataT>(i);

		for( unsigned int i = 0; i < rows * columns; ++i )
			Mat[i] = tmp.Mat[i];

	};
#endif

protected:
	DataT Mat[rows * columns];
};

#pragma endregion
//////////////////////////
//Mat4 decl
#pragma region Matrix4x4 specialization
template <
			class DataT
		 >
class ThFixedMatContainer<DataT,4,4,generic_mat_tag>
{
	typedef ThFixedMatrix<DataT,4,4,generic_mat_tag> mat_t;
	typedef ThFixedMatrix<DataT,3,3,generic_mat_tag> mat3_t;
	typedef ThFixedVector<DataT,3,generic_vec_tag> vec3_t;
public:
	
	THOR_INLINE ThFixedMatContainer():
		m00( DataT(1.0) ), m01( DataT(0.0) ), m02( DataT(0.0) ), m03( DataT(0.0) ),
		m10( DataT(0.0) ), m11( DataT(1.0) ), m12( DataT(0.0) ), m13( DataT(0.0) ),
		m20( DataT(0.0) ), m21( DataT(0.0) ), m22( DataT(1.0) ), m23( DataT(0.0) ),
		m30( DataT(0.0) ), m31( DataT(0.0) ), m32( DataT(0.0) ), m33( DataT(1.0) )
	{};
	
	//THOR_INLINE ThFixedMatContainer( const ThFixedMatContainer& copy ):
	//	m00( copy.m00 ), m01( copy.m01 ), m02( copy.m02 ), m03( copy.m03 ),
	//	m10( copy.m10 ), m11( copy.m11 ), m12( copy.m12 ), m13( copy.m13 ),
	//	m20( copy.m20 ), m21( copy.m21 ), m22( copy.m22 ), m23( copy.m23 ),
	//	m30( copy.m30 ), m31( copy.m31 ), m32( copy.m32 ), m33( copy.m33 )
	//{};

	THOR_INLINE ThFixedMatContainer
				( 
					const DataT& m00_,  const DataT& m01_,  const DataT& m02_,  const DataT& m03_,
					const DataT& m10_,  const DataT& m11_,  const DataT& m12_,  const DataT& m13_,
					const DataT& m20_,  const DataT& m21_,  const DataT& m22_,  const DataT& m23_,
					const DataT& m30_,  const DataT& m31_,  const DataT& m32_,  const DataT& m33_
				)
		:	m00( m00_ ), m01( m01_ ), m02( m02_ ), m03( m03_ ),
			m10( m10_ ), m11( m11_ ), m12( m12_ ), m13( m13_ ),
			m20( m20_ ), m21( m21_ ), m22( m22_ ), m23( m23_ ),
			m30( m30_ ), m31( m31_ ), m32( m32_ ), m33( m33_ )
	{};

	vec3_t GetTranslation()const
	{
		return vec3_t(m30,m31,m32);
	}

	void SetTranslation(const vec3_t& val)
	{
		m30 = val(0);
		m31 = val(1);
		m32 = val(2);
	}

	mat3_t ToMat3x3()const
	{
		return mat3_t(	m00,m01,m02,
						m10,m11,m12,
						m20,m21,m22
						);
	}

	THOR_INLINE mat_t&
	PerspectiveDX( DataT fovy, DataT aspect, DataT zNear, DataT zFar )
	{
		mat_t* ptr = (mat_t*)this;
		PerspectiveDXImpl( *ptr, fovy, aspect, zNear, zFar );
		return *ptr;
	}	

	THOR_INLINE mat_t&
	OrthoDX( DataT left, DataT right, DataT bottom, DataT top, DataT zNear, DataT zFar )
	{
		mat_t* ptr = (mat_t*)this;
		OrthoDXImpl( *ptr, left, right, bottom, top, zNear, zFar );
		return *ptr;
	}
	
	THOR_INLINE mat_t&
	Perspective( DataT fovy, DataT aspect, DataT zNear, DataT zFar )
	{
		mat_t* ptr = (mat_t*)this;
		PerspectiveImpl( *ptr, fovy, aspect, zNear, zFar );
		return *ptr;
	}	

	THOR_INLINE mat_t&
	Ortho( DataT left, DataT right, DataT bottom, DataT top, DataT zNear, DataT zFar )
	{
		mat_t* ptr = (mat_t*)this;
		OrthoImpl( *ptr, left, right, bottom, top, zNear, zFar );
		return *ptr;
	}	

	template <class VecT>
	THOR_INLINE mat_t& 
	LookAt( const VecT& eye , const VecT& center , const VecT& up )
	{
		mat_t* ptr = (mat_t*)this;
		LookAtImpl( *ptr, eye, center, up );
		return *ptr;
	}

	template <class VecT>
	THOR_INLINE mat_t& 
	Translate( const VecT& t )
	{
		mat_t* ptr=(mat_t*)this;
		TranslateImpl( *ptr, t );
		return *ptr;
	}

	template <class VecT>
	THOR_INLINE mat_t& 
	Scale( const VecT& s )
	{
		mat_t* ptr = (mat_t*)this;
		ScaleImpl( *ptr, s );
		return *ptr;
	}
    
    THOR_INLINE mat_t&
    Scale( const DataT& s )
    {
        mat_t* ptr = (mat_t*)this;
        ScaleImpl( *ptr, s );
        return *ptr;
    }
    
    THOR_INLINE mat_t& 
	RotateX( DataT angle )
	{
		mat_t* ptr = (mat_t*)this;
		RotateXImpl( *ptr, angle );
		return *ptr;
	}

	THOR_INLINE mat_t& 
	RotateY( DataT angle )
	{
		mat_t* ptr = (mat_t*)this;
		RotateYImpl( *ptr, angle );
		return *ptr;
	}

	THOR_INLINE mat_t& 
	RotateZ( DataT angle )
	{
		mat_t* ptr = (mat_t*)this;
		RotateZImpl( *ptr, angle );
		return *ptr;
	}

	template <class VecT>
	THOR_INLINE mat_t& 
	Rotate( DataT angle, const VecT& axis )
	{
		mat_t* ptr = (mat_t*)this;
		RotateImpl ( *ptr, angle, axis );
		return *ptr;
	}	
#ifdef USE_MATRIX_EXPRESSION_TEMPLATES
	template < class ExprT >
	THOR_INLINE void assign_expr( const ExprT& e )
	{
		ThFixedMatContainer tmp;
	
		for( unsigned int i = 0; i < 16; ++i )
			tmp.Mat[i] = e.tree.at<DataT>(i);

		for( unsigned int i = 0; i < 16; ++i )
			Mat[i] = tmp.Mat[i];
	};
#endif
	
protected:
	union
	{
		DataT Mat[16];
	
		struct
		{
			DataT	m00,m01,m02,m03,
					m10,m11,m12,m13,
					m20,m21,m22,m23,
					m30,m31,m32,m33;
		};
	};
};
#pragma endregion

#pragma region Matrix3x3 specialization
template <
			class DataT
		 >
class ThFixedMatContainer<DataT,3,3,generic_mat_tag>
{
	typedef ThFixedVector< DataT,4,quaternion_tag > quat_t;
public:
	
	THOR_INLINE ThFixedMatContainer():
		m00( DataT(1.0) ), m01( DataT(0.0) ), m02( DataT(0.0) ),
		m10( DataT(0.0) ), m11( DataT(1.0) ), m12( DataT(0.0) ),
		m20( DataT(0.0) ), m21( DataT(0.0) ), m22( DataT(1.0) )
	{};
	
	//THOR_INLINE ThFixedMatContainer( const ThFixedMatContainer& copy ):
	//	m00( copy.m00 ), m01( copy.m01 ), m02( copy.m02 ),
	//	m10( copy.m10 ), m11( copy.m11 ), m12( copy.m12 ),
	//	m20( copy.m20 ), m21( copy.m21 ), m22( copy.m22 )
	//{};

	THOR_INLINE ThFixedMatContainer
				( 
					const DataT& m00_,  const DataT& m01_,  const DataT& m02_,  
					const DataT& m10_,  const DataT& m11_,  const DataT& m12_,  
					const DataT& m20_,  const DataT& m21_,  const DataT& m22_  
				)
		:	m00( m00_ ), m01( m01_ ), m02( m02_ ), 
			m10( m10_ ), m11( m11_ ), m12( m12_ ), 
			m20( m20_ ), m21( m21_ ), m22( m22_ )
	{};

	quat_t ToQuat()const
	{
		// Output quaternion
		DataT w,x,y,z;
		// Determine which of w, x, y, or z has the largest absolute value
		DataT fourWSquaredMinus1 = m00 + m11 + m22;
		DataT fourXSquaredMinus1 = m00 - m11 - m22;
		DataT fourYSquaredMinus1 = m11 - m00 - m22;
		DataT fourZSquaredMinus1 = m22 - m00 - m11;
		int biggestIndex = 0;

		DataT fourBiggestSquaredMinus1 = fourWSquaredMinus1;
		
		if (fourXSquaredMinus1 > fourBiggestSquaredMinus1)
		{
			fourBiggestSquaredMinus1 = fourXSquaredMinus1;
			biggestIndex = 1;
		}

		if (fourYSquaredMinus1 > fourBiggestSquaredMinus1) 
		{
			fourBiggestSquaredMinus1 = fourYSquaredMinus1;
			biggestIndex = 2;
		}

		if (fourZSquaredMinus1 > fourBiggestSquaredMinus1)
		{
			fourBiggestSquaredMinus1 = fourZSquaredMinus1;
			biggestIndex = 3;
		}

		// Perform square root and division
		DataT biggestVal = sqrt(fourBiggestSquaredMinus1 + DataT(1.0)) * DataT(0.5);
		DataT mult = DataT(0.25) / biggestVal;
		// Apply table to compute quaternion values
		switch (biggestIndex) 
		{
		case 0:
		w = biggestVal;
		x = (m12 - m21) * mult;
		y = (m20 - m02) * mult;
		z = (m01 - m10) * mult;
		break;
		case 1:
		x = biggestVal;
		w = (m12 - m21) * mult;
		y = (m01 + m10) * mult;
		z = (m20 + m02) * mult;
		break;
		case 2:
		y = biggestVal;
		w = (m20 - m02) * mult;
		x = (m01 + m10) * mult;
		z = (m12 + m21) * mult;
		break;
		case 3:
		z = biggestVal;
		w = (m01 - m10) * mult;
		x = (m20 + m02) * mult;
		y = (m12 + m21) * mult;
		break;
		}

		quat_t res;
		res.x() = x;
		res.y() = y;
		res.z() = z;
		res.w() = w;

		return res;
	}
#ifdef USE_MATRIX_EXPRESSION_TEMPLATES
	template < class ExprT >
	THOR_INLINE void assign_expr( const ExprT& e )
	{
		ThFixedMatContainer tmp;
	
		for( unsigned int i = 0; i < 9; ++i )
			tmp.Mat[i] = e.tree.at<DataT>(i);

		for( unsigned int i = 0; i < 9; ++i )
			Mat[i] = tmp.Mat[i];

	};
#endif
	
protected:
	union
	{
		DataT Mat[9];
	
		struct
		{
			DataT	m00,m01,m02,
					m10,m11,m12,
					m20,m21,m22;
		};
	};
};
#pragma endregion

#pragma region Matrix2x2 specialization
template <
			class DataT
		 >
class ThFixedMatContainer<DataT,2,2,generic_mat_tag>
{
public:
	
	THOR_INLINE ThFixedMatContainer():
		m00( DataT(1.0) ), m01( DataT(0.0) ),
		m10( DataT(0.0) ), m11( DataT(1.0) )
	{};
	
	//THOR_INLINE ThFixedMatContainer( const ThFixedMatContainer& copy ):
	//	m00( copy.m00 ), m01( copy.m01 ),
	//	m10( copy.m10 ), m11( copy.m11 )
	//{};

	THOR_INLINE ThFixedMatContainer
				( 
					const DataT& m00_,  const DataT& m01_,
					const DataT& m10_,  const DataT& m11_  
				)
		:	m00( m00_ ), m01( m01_ ),
			m10( m10_ ), m11( m11_ )
	{};
#ifdef USE_MATRIX_EXPRESSION_TEMPLATES
	template < class ExprT >
	THOR_INLINE void assign_expr( const ExprT& e )
	{
		ThFixedMatContainer tmp;
	
		for( unsigned int i = 0; i < 4; ++i )
			tmp.Mat[i] = e.tree.at<DataT>(i);

		for( unsigned int i = 0; i < 4; ++i )
			Mat[i] = tmp.Mat[i];

	};
#endif
	
protected:
	union
	{
		DataT Mat[4];
	
		struct
		{
			DataT	m00,m01,
					m10,m11;
		};
	};
};
#pragma endregion
/////////////////////////
template < 
			class DataT, unsigned int rows, unsigned int columns, //rows x columns matrix
			class TagT
		 >
class ThFixedMatrix : public ThFixedMatContainer< DataT, rows, columns, TagT >
{
public:
	typedef ThFixedMatContainer< DataT, rows, columns, TagT > container_t;
	typedef ThFixedMatrix< DataT, rows, columns, TagT > self_t;
	typedef DataT value_type;


	typedef DataT* iterator;
	typedef const DataT* const_iterator;

	ThFixedMatrix( const_iterator begin, const_iterator end )
	{
		const_iterator i;
		iterator j;
		for(i = begin, j = Begin(); i != end, j != End(); ++i,++j)
			*j = *i;
	}

	ThFixedMatrix():container_t(){};

	ThFixedMatrix( const DataT& v0 )
		:container_t(v0)
	{};

	ThFixedMatrix( const DataT& v0, const DataT& v1 )
		:container_t(v0,v1)
	{};

	ThFixedMatrix( const DataT& v0, const DataT& v1, const DataT& v2 )
		:container_t(v0,v1,v2)
	{};

	ThFixedMatrix( const DataT& v0, const DataT& v1, const DataT& v2, const DataT& v3 )
		:container_t(v0,v1,v2,v3)
	{};

	ThFixedMatrix( 
					const DataT& v0, const DataT& v1, const DataT& v2, const DataT& v3,
					const DataT& v4
				)
		:container_t(v0,v1,v2,v3,v4)
	{};

	ThFixedMatrix( 
					const DataT& v0, const DataT& v1, const DataT& v2, const DataT& v3,
					const DataT& v4, const DataT& v5
				)
		:container_t(v0,v1,v2,v3,v4,v5)
	{};

	ThFixedMatrix( 
					const DataT& v0, const DataT& v1, const DataT& v2, const DataT& v3,
					const DataT& v4, const DataT& v5, const DataT& v6
				)
		:container_t(v0,v1,v2,v3,v4,v5,v6)
	{};

	ThFixedMatrix( 
					const DataT& v0, const DataT& v1, const DataT& v2, const DataT& v3,
					const DataT& v4, const DataT& v5, const DataT& v6, const DataT& v7
				)
		:container_t(v0,v1,v2,v3,v4,v5,v6,v7)
	{};

	ThFixedMatrix( 
					const DataT& v0, const DataT& v1, const DataT& v2, const DataT& v3,
					const DataT& v4, const DataT& v5, const DataT& v6, const DataT& v7,
					const DataT& v8
				)
		:container_t(v0,v1,v2,v3,v4,v5,v6,v7,v8)
	{};

	ThFixedMatrix( 
					const DataT& v0, const DataT& v1, const DataT& v2, const DataT& v3,
					const DataT& v4, const DataT& v5, const DataT& v6, const DataT& v7,
					const DataT& v8, const DataT& v9
				)
		:container_t(v0,v1,v2,v3,v4,v5,v6,v7,v8,v9)
	{};

	ThFixedMatrix( 
					const DataT& v0, const DataT& v1, const DataT& v2, const DataT& v3,
					const DataT& v4, const DataT& v5, const DataT& v6, const DataT& v7,
					const DataT& v8, const DataT& v9, const DataT& v10
				)
		:container_t(v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10)
	{};

	ThFixedMatrix( 
					const DataT& v0,  const DataT& v1,  const DataT& v2,  const DataT& v3,
					const DataT& v4,  const DataT& v5,  const DataT& v6,  const DataT& v7,
					const DataT& v8,  const DataT& v9,  const DataT& v10, const DataT& v11
				)
		:container_t(v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11)
	{};

	ThFixedMatrix( 
					const DataT& v0,  const DataT& v1,  const DataT& v2,  const DataT& v3,
					const DataT& v4,  const DataT& v5,  const DataT& v6,  const DataT& v7,
					const DataT& v8,  const DataT& v9,  const DataT& v10, const DataT& v11,
					const DataT& v12
				)
		:container_t(v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12)
	{};

	ThFixedMatrix( 
					const DataT& v0,  const DataT& v1,  const DataT& v2,  const DataT& v3,
					const DataT& v4,  const DataT& v5,  const DataT& v6,  const DataT& v7,
					const DataT& v8,  const DataT& v9,  const DataT& v10, const DataT& v11,
					const DataT& v12, const DataT& v13
				)
		:container_t(v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13)
	{};
	
	ThFixedMatrix( 
					const DataT& v0,  const DataT& v1,  const DataT& v2,  const DataT& v3,
					const DataT& v4,  const DataT& v5,  const DataT& v6,  const DataT& v7,
					const DataT& v8,  const DataT& v9,  const DataT& v10, const DataT& v11,
					const DataT& v12, const DataT& v13, const DataT& v14
				)
		:container_t(v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14)
	{};

	ThFixedMatrix( 
					const DataT& v0,  const DataT& v1,  const DataT& v2,  const DataT& v3,
					const DataT& v4,  const DataT& v5,  const DataT& v6,  const DataT& v7,
					const DataT& v8,  const DataT& v9,  const DataT& v10, const DataT& v11,
					const DataT& v12, const DataT& v13, const DataT& v14, const DataT& v15
				)
		:container_t(v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15)
	{};
	
	//ThFixedMatrix( const ThFixedMatrix& copy ):container_t(copy){};
	//assigns all elements of the vector to s
	template < class RealT >
	inline ThListConstructor< iterator, DataT > operator=(const RealT& s)
	{
		return ThListConstructor< iterator, DataT >( &container_t::Mat[0], s, 0, Size() );
	};
	//
#ifdef USE_MATRIX_EXPRESSION_TEMPLATES	
	template <class T, class R>
	THOR_INLINE  const ThFixedMatrix& operator=( const Expression<T,R>& e)
	{		
		assign_expr(e);		
		return *this;
	};
	//
	
#else
	
	//Mat+Mat
	template< class _T, class _TagT >
	THOR_INLINE ThFixedMatrix operator+(const ThFixedMatrix<_T,rows,columns,_TagT> &m) const
	{
		ThFixedMatrix tmp;
		MatAdd( *this, m, tmp );
		return tmp;
	};
	//Mat-Mat
	template< class _T, class _TagT >
	THOR_INLINE ThFixedMatrix operator-(const ThFixedMatrix<_T,rows,columns,_TagT> &m) const
	{
		ThFixedMatrix tmp;
		MatSub( *this, m, tmp );
		return tmp;
	};
	//Mat*Mat
	template< class _T, class _TagT >
	THOR_INLINE ThFixedMatrix operator*(const ThFixedMatrix<_T,rows,columns,_TagT> &m) const
	{
		ThFixedMatrix tmp;
		MatMulMat( *this, m, tmp );
		return tmp;
	};
	//Mat*Vec
	template< class TV, class TagTV >
	THOR_INLINE 
	ThFixedVector<TV,columns,TagTV>
	operator*(const ThFixedVector<TV,columns,TagTV> &v) const
	{
		ThFixedVector<TV,columns,TagTV> tmp;
		MatMulVec( *this, v, tmp );
		return tmp;
	};
	//Mat*scalar
	THOR_INLINE ThFixedMatrix operator*(const value_type &s) const
	{
		ThFixedMatrix tmp;
		MatMulScalar( *this, s, tmp );
		return tmp;
	};
	//Mat/scalar
	THOR_INLINE ThFixedMatrix operator/(const value_type &s) const
	{
		ThFixedMatrix tmp;
		MatDivScalar( *this, s, tmp );
		return tmp;
	};	
	//-Mat
	template< class _T, class _TagT >
	friend THOR_INLINE ThFixedMatrix operator-(const ThFixedMatrix<_T,rows,columns,_TagT>& arg)
	{
		ThFixedMatrix tmp;
		MatUnaryMinus(arg,tmp);
		return tmp;
	};
#endif
	//Mat=Mat
	template< class _T, class _TagT >
	THOR_INLINE ThFixedMatrix& operator=(const ThFixedMatrix<_T,rows,columns,_TagT> &m)
	{
		MatAssign( *this, m );
		return *this;
	};
	//Mat+=Mat
	template< class _T, class _TagT >
	THOR_INLINE ThFixedMatrix& operator+=(const ThFixedMatrix<_T,rows,columns,_TagT> &m)
	{
		MatAdd( *this, m, *this );
		return *this;
	};
	//Mat-=Mat
	template< class _T, class _TagT >
	THOR_INLINE ThFixedMatrix& operator-=(const ThFixedMatrix<_T,rows,columns,_TagT> &m)
	{
		MatSub( *this, m, *this );
		return *this;
	};
	//Mat*=Mat
	template< class _T, class _TagT >
	THOR_INLINE ThFixedMatrix& operator*=(const ThFixedMatrix<_T,rows,columns,_TagT> &m)
	{
		ThFixedMatrix tmp(*this);
		MatMulMat( tmp, m, *this );
		return *this;
	};	
	//Mat*=scalar
	//template< class RealT >
	THOR_INLINE ThFixedMatrix& operator*=(const value_type& s)
	{
		MatMulScal( *this, s, *this );
		return *this;
	};
	//Mat/=scalar
	//template< class RealT >
	THOR_INLINE ThFixedMatrix& operator/=(const value_type& s)
	{
		MatDivScal( *this, s, *this );
		return *this;
	};

	THOR_INLINE const DataT& operator()( const unsigned int i ) const
	{
		return container_t::Mat[i];
	};

	THOR_INLINE DataT& operator()( const unsigned int i )
	{
		return container_t::Mat[i];
	};
	//block accessor
	THOR_INLINE const DataT& operator()( const unsigned int i, const unsigned int j ) const
	{	
#ifdef ROW_MAJOR_MATRIX
		return container_t::Mat[i * columns + j];
#elif  defined(COLUMN_MAJOR_MATRIX)
		return container_t::Mat[ j * rows + i];
#endif
	};
	//block accessor
	THOR_INLINE DataT& operator()( const unsigned int i, const unsigned int j )
	{	
#ifdef ROW_MAJOR_MATRIX
		return container_t::Mat[i * columns + j];
#elif  defined(COLUMN_MAJOR_MATRIX)
		return container_t::Mat[j * rows + i];
#endif
	};
	//checks if matrices are equal
	THOR_INLINE bool  operator==( const ThFixedMatrix &m) const
	{
		return AreMatEqual( *this, m );
	};
	//returns the size of the vector
	THOR_INLINE const unsigned int Size(void) const
	{	
		return rows * columns;
	};
	
	//returns an iterator pointing to the first element
	THOR_INLINE const_iterator Begin(void) const
	{	
		return &container_t::Mat[0];
	};
	//returns an iterator pointing to the element behind the last
	THOR_INLINE const_iterator End(void) const
	{	
		return &container_t::Mat[rows * columns];
	};

	//returns an iterator pointing to the first element
	THOR_INLINE iterator Begin(void)
	{	
		return &container_t::Mat[0];
	};
	//returns an iterator pointing to the element behind the last
	THOR_INLINE iterator End(void)
	{	
		return &container_t::Mat[rows * columns];
	};

	THOR_INLINE unsigned int Rows() const
	{
		return rows;
	}

	THOR_INLINE unsigned int Columns() const
	{
		return columns;
	}

	THOR_INLINE DataT Inverse()
	{
		return InverseImpl( *this);
	}

	THOR_INLINE DataT Determinant()
	{
		return DeterminantImpl( *this );
	}

	THOR_INLINE ThFixedMatrix& Transpose()
	{
		TransposeImpl( *this );
		return *this;
	}

	THOR_INLINE ThFixedMatrix& LoadIdentity()
	{
		LoadIdentityImpl( *this );
		return *this;
	}

	THOR_INLINE ThFixedMatrix& LoadDiag(DataT d)
	{
		LoadDiagImpl( *this,d );
		return *this;
	}

	THOR_INLINE bool IsRowMajor()const
	{
#ifdef ROW_MAJOR_MATRIX
		return true;
#elif  defined(COLUMN_MAJOR_MATRIX)
		return false;
#endif
	}


};

//Vec*Mat
template< class TV, class TagTV, class T, class TagT, unsigned int rows, unsigned int columns >
THOR_INLINE ThFixedVector<TV,rows,TagTV>
operator*(const ThFixedVector<TV,rows,TagTV> &v, const ThFixedMatrix<T,rows,columns,TagT> &m)
{
	ThFixedVector<TV,rows,TagTV> tmp;
	VecMulMat( v, m, tmp );
	return tmp;
};
//scalar*Mat
template< class T, class TagT, class RealT, unsigned int rows, unsigned int columns >
THOR_INLINE ThFixedMatrix<T,rows,columns,TagT>
operator*(const RealT s,const ThFixedMatrix<T,rows,columns,TagT> &m)
{
	ThFixedMatrix<T,rows,columns,TagT> tmp;
	MatMulScalar( m, s, tmp );
	return tmp;
};
#ifdef USE_MATRIX_EXPRESSION_TEMPLATES
template<>
struct binary_result< ThMatMulVecTag,ThFixedMatrix<float,4,4>,ThFixedVector<float,4> >
{
	typedef ThFixedVector<float,4> result_t;
};

//binary plus
THOR_INLINE MakeNode< ThMatMulVecTag, ThFixedMatrix<float,4,4>, ThFixedVector<float,4> >::expr_t
operator*(const ThFixedMatrix<float,4,4>& lhs, const ThFixedVector<float,4>& rhs)
{
	typedef MakeNode< ThMatMulVecTag, ThFixedMatrix<float,4,4>, ThFixedVector<float,4> >::node_t node_t;
	typedef MakeNode< ThMatMulVecTag, ThFixedMatrix<float,4,4>, ThFixedVector<float,4> >::expr_t expr_t;

	return expr_t( node_t( Leaf< ThFixedMatrix<float,4,4> >(lhs), Leaf< ThFixedVector<float,4> >(rhs) ) );
};
#endif


}//Thor