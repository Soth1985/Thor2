
template< class MatT >
THOR_INLINE void MatUnaryMinus( const MatT& arg, MatT& res )
{
	for ( unsigned int i = 0; i < arg.Rows(); ++i )
	{
		for ( unsigned int j = 0; j < arg.Columns(); ++j )
			res(i,j) = -arg(i,j);
	}
};

//adds two generic matrices
template< class MatT1, class MatT2, class ResT >
THOR_INLINE void MatAdd( const MatT1& lhs, const MatT2& rhs, ResT& res )
{
	for ( unsigned int i = 0; i < lhs.Rows(); ++i )
	{
		for ( unsigned int j = 0; j < lhs.Columns(); ++j )
			res(i,j) = lhs(i,j) + rhs(i,j);
	}
};

//subcribes two generic matrices
template< class MatT1, class MatT2, class ResT >
THOR_INLINE void MatSub( const MatT1& lhs, const MatT2& rhs, ResT& res )
{
	for ( unsigned int i = 0; i < lhs.Rows(); ++i )
	{
		for ( unsigned int j = 0; j < lhs.Columns(); ++j )
			res(i,j) = lhs(i,j) - rhs(i,j);
	}
};

//multilplies matrix lhs by a scalar rhs
template< class MatT, class ResT, class RealT >
THOR_INLINE void MatMulScalar( const MatT& lhs, const RealT& rhs, ResT& res )
{
	for ( unsigned int i = 0; i < lhs.Rows(); ++i )
	{
		for ( unsigned int j = 0; j < lhs.Columns(); ++j )
			res(i,j) = lhs(i,j) * rhs;
	}
};

//divides matrix lhs by a scalar rhs
template< class MatT, class ResT, class RealT >
THOR_INLINE void MatDivScalar( const MatT& lhs, const RealT& rhs, ResT& res )
{
	for ( unsigned int i = 0; i < lhs.Rows(); ++i )
	{
		for ( unsigned int j = 0; j < lhs.Columns(); ++j )
			res(i,j) = lhs(i,j) / rhs;
	}
};

//checks for equality of two generic matrices
template< class MatT1, class MatT2 >
THOR_INLINE bool AreMatEqual( const MatT1& lhs, const MatT2& rhs )
{
	for ( unsigned int i = 0; i < lhs.Rows(); ++i )
	{
		for ( unsigned int j = 0; j < lhs.Columns(); ++j )
		{
			if ( Math::Abs( lhs(i,j) - rhs(i,j) ) > Math::E4 )
			return false;
		}
	}

	return true;
};

//assigns lhs matrix to rhs
template< class MatT1, class MatT2 >
THOR_INLINE void MatAssign( MatT1& lhs, const MatT2& rhs )
{
	for ( unsigned int i = 0; i < lhs.Rows(); ++i )
	{
		for ( unsigned int j = 0; j < lhs.Columns(); ++j )
			lhs(i,j) = rhs(i,j);
	}
};

//assigns lhs to rhs
template< class LhsT, class RealT >
THOR_INLINE void MatAssignScalar( LhsT& lhs, const RealT& rhs )
{
	for ( unsigned int i = 0; i < lhs.Rows(); ++i )
	{
		for ( unsigned int j = 0; j < lhs.Columns(); ++j )
			lhs(i,j) = rhs;
	}
};

//vec=mat*vec
template< class MatT, class VecT, class ResT >
THOR_INLINE void MatMulVec( const MatT& lhs, const VecT& rhs, ResT& res )
{
	typedef typename ResT::value_type value_type;
	//unsigned int k = 0;
	for ( unsigned int i = 0; i < lhs.Rows(); ++i )
	{
		//res(i) = value_type(0.0);
		value_type tmp(0.0);
		for ( unsigned int j = 0; j < lhs.Columns(); ++j )
			tmp += lhs(i,j) * rhs(j);
		res(i) = tmp;
		//k++;
	}
};

//vec=vec*mat
template< class MatT, class VecT, class ResT >
THOR_INLINE void VecMulMat( const VecT& lhs, const MatT& rhs, ResT& res )
{
	typedef typename ResT::value_type value_type;
	for ( unsigned int i = 0; i < rhs.Columns(); ++i )
	{
		//res(i)=value_type(0.0);
		value_type tmp(0.0);
		for ( unsigned int j = 0; j < rhs.Rows(); ++j )
			tmp += rhs(j,i) * lhs(j);
		res(i) = tmp;
	}
};

//mat=mat*mat
template< class MatT1, class MatT2, class ResT >
THOR_INLINE void MatMulMat( const MatT1& lhs, const MatT2& rhs, ResT& res )
{
	
	typedef typename ResT::value_type value_type;
	for ( unsigned int i = 0; i < lhs.Rows(); ++i )
	{		
		for ( unsigned int j = 0; j < rhs.Columns(); ++j )
		{
			value_type tmp = value_type(0.0);
			for ( unsigned int k = 0; k < rhs.Rows(); ++k )
				tmp += lhs(i,k) * rhs(k,j);
			res(i,j) = tmp;
		}
	}
};

//load identity matrix
/*!
 * \brief
 * Loads identity matrix.
 * 
 * \param arg
 * Arg is the target matrix.
 *
 * \returns
 * Returns identity matrix.
 *  
 * 
 * An identity matrix is loaded into arg. Arg can be non square. All elements on main diag will be 1.0
 * , and zeros everywhere else.
 * 
 */
template< class MatT >
THOR_INLINE void LoadIdentityImpl( MatT& arg )
{
	typedef typename MatT::value_type value_type;
	for ( unsigned int i = 0; i < arg.Rows(); ++i )
	{
		for ( unsigned int j = 0; j < arg.Columns(); ++j )
		{
			if ( i == j )
				arg(i,j) = value_type(1.0);
			else 
				arg(i,j) = value_type(0.0);
		}
	}

	//return arg;
};
/*!
 * \brief
 * Load diagonal matrix.
 * 
 * \param arg
 * Arg is the target matrix.
 * 
 * \param d
 * d is the value to be set on main diagonal.
 *
 * \returns
 * Returns diagonal matrix.
 *  
 * 
 * An diagonal matrix is loaded into arg. Arg can be non square. All elements on main diag will be d
 * , and zeros everywhere else.
 * 
 */
template< class MatT, class RealT >
THOR_INLINE void LoadDiagImpl( MatT& arg, const RealT d )
{
	typedef typename MatT::value_type value_type;
	for ( unsigned int i = 0; i < arg.Rows(); ++i )
	{
		for ( unsigned int j = 0; j < arg.Columns(); ++j )
		{
			if ( i == j )
				arg(i,j) = value_type(d);
			else 
				arg(i,j) = value_type(0.0);
		}
	}

	//return arg;
};


/*!
 * \brief
 * Transposes a matrix.
 * 
 * \param arg
 * Arg is target matrix.
 * 
 * \returns
 * Returns transposed arg matrix.
 * 
 * 
 * This function works only with square matrices.
 * 
 * 
 */
template< class T, unsigned int r, class TagT >
THOR_INLINE void TransposeImpl( ThFixedMatrix<T,r,r,TagT>& arg )
{
	for ( unsigned int i = 0; i < r; ++i )
	{
		for ( unsigned int j = (i + 1); j < r; ++j )
		{
			Swap( arg(i,j), arg(j,i) );
		}
	}

	//return arg;
};

/*!
 * \brief
 * Transposes a matrix.
 * 
 * \param arg
 * Arg is target matrix.
 * 
 * \returns
 * Returns transposed arg matrix.
 * 
 * 
 * This function works with all matrices.
 * 
 * 
 */
template< class T, unsigned int r, unsigned int c, class TagT >
THOR_INLINE void TransposeImpl( ThFixedMatrix<T,r,c,TagT>& arg )
{
	ThFixedMatrix<T,c,r,TagT> temp;
	for ( unsigned int i = 0; i < arg.Rows(); ++i )
	{
		for ( unsigned int j = 0; j < arg.Coloumns(); ++j )
		{
			arg(j,i) = temp(i,j);
		}
	}

	//return temp;
};

template< class T, unsigned int r, class TagT >
THOR_INLINE T 
DeterminantImpl( ThFixedMatrix<T,r,r,TagT>& arg )
{
	
	///TODO: implement generic matrix determinant calculation
};

template< class T, class TagT >
THOR_INLINE T 
DeterminantImpl( ThFixedMatrix<T,4,4,TagT>& arg )
{
	
	typedef ThFixedMatrix<T,4,4,TagT> mat_t;
	typedef typename mat_t::value_type value_type;
	value_type pairs[6], diff[3], tM[4];
	//mat_t tM;
		
		
	//calculate first 8 elements
	pairs[0] = arg(2,2) * arg(3,3);
	pairs[1] = arg(3,2) * arg(2,3);
	pairs[2] = arg(1,2) * arg(3,3);
	pairs[3] = arg(1,3) * arg(3,2);
	pairs[4] = arg(1,2) * arg(2,3);
	pairs[5] = arg(2,2) * arg(1,3);

	diff[0] = pairs[0] - pairs[1];
	diff[1] = pairs[2] - pairs[3];
	diff[2] = pairs[4] - pairs[5];	

	tM[0] = arg(1,1) * diff[0] - arg(2,1) * diff[1] + arg(3,1) * diff[2];
	tM[1] =-arg(1,0) * diff[0] + arg(2,0) * diff[1] - arg(3,0) * diff[2];
	
	//calculate second 8 elements
	pairs[0] = arg(2,0) * arg(3,1);
	pairs[1] = arg(2,1) * arg(3,0);
	pairs[2] = arg(1,0) * arg(3,1);
	pairs[3] = arg(1,1) * arg(3,0);
	pairs[4] = arg(1,0) * arg(2,1);
	pairs[5] = arg(2,0) * arg(1,1);

	diff[0] = pairs[0] - pairs[1];
	diff[1] = pairs[2] - pairs[3];
	diff[2] = pairs[4] - pairs[5];	

	tM[2] = arg(1,3) * diff[0] - arg(2,3) * diff[1] + arg(3,3) * diff[2];
	tM[3] =-arg(1,2) * diff[0] + arg(2,2) * diff[1] - arg(3,2) * diff[2];

	return tM[0] * arg(0,0) + tM[1] * arg(0,1) + tM[2] * arg(0,2) + tM[3] * arg(0,3);
};

template< class T, class TagT >
THOR_INLINE T 
DeterminantImpl( ThFixedMatrix<T,3,3,TagT>& arg )
{
	return	arg(0,0) * ( arg(1,1) * arg(2,2) - arg(1,2) * arg(2,1) ) +
			arg(0,1) * ( arg(1,2) * arg(2,0) - arg(1,0) * arg(2,2) ) +
			arg(0,2) * ( arg(1,0) * arg(2,1) - arg(1,1) * arg(2,0) );
}

template< class T, class TagT >
THOR_INLINE T 
DeterminantImpl( ThFixedMatrix<T,2,2,TagT>& arg )
{
	return arg(0,0) * arg(1,1) - arg(0,1) * arg(1,0);
}

/*!
 * \brief
 * Inverts a martix.
 * 
 * \param arg
 * Arg is target matrix.
  * \param det
 * det is a computed determinant.
 * 
 * \returns
 * Returns inverted arg matrix.
 * 
 * This function works only with square matrices.
 * 
 */
template< class T, unsigned int r, class TagT >
THOR_INLINE T 
InverseImpl( ThFixedMatrix<T,r,r,TagT>& arg)
{
	
	///TODO: implement generic matrix inversion algorithm

	//return arg;
};

//invert a 4x4 matrix
template< class T, class TagT >
THOR_INLINE T
InverseImpl( ThFixedMatrix<T,4,4,TagT>& arg )
{
	typedef ThFixedMatrix<T,4,4,TagT> mat_t;
	typedef typename mat_t::value_type value_type;
	value_type det, resdet;
	value_type pairs[6], diff[6];
	mat_t tM;
		
		
		//calculate first 8 elements
		pairs[0] = arg(2,2) * arg(3,3);
		pairs[1] = arg(3,2) * arg(2,3);
		pairs[2] = arg(1,2) * arg(3,3);
		pairs[3] = arg(1,3) * arg(3,2);
		pairs[4] = arg(1,2) * arg(2,3);
		pairs[5] = arg(2,2) * arg(1,3);

		diff[0] = pairs[0] - pairs[1];
		diff[1] = pairs[2] - pairs[3];
		diff[2] = pairs[4] - pairs[5];

		pairs[0] = arg(0,2) * arg(3,3);
		pairs[1] = arg(3,2) * arg(0,3);
		pairs[2] = arg(0,2) * arg(2,3);
		pairs[3] = arg(0,3) * arg(2,2);
		pairs[4] = arg(0,2) * arg(1,3);
		pairs[5] = arg(0,3) * arg(1,2);

		diff[3] = pairs[0] - pairs[1];
		diff[4] = pairs[2] - pairs[3];
		diff[5] = pairs[4] - pairs[5];

		tM(0,0) = arg(1,1) * diff[0] - arg(2,1) * diff[1] + arg(3,1) * diff[2];
		tM(1,0) =-arg(0,1) * diff[0] + arg(2,1) * diff[3] - arg(3,1) * diff[4];
		tM(2,0) = arg(0,1) * diff[1] - arg(1,1) * diff[3] + arg(3,1) * diff[5];
		tM(3,0) =-arg(0,1) * diff[2] + arg(1,1) * diff[4] - arg(2,1) * diff[5];

		tM(0,1) =-arg(1,0) * diff[0] + arg(2,0) * diff[1] - arg(3,0) * diff[2];
		tM(1,1) = arg(0,0) * diff[0] - arg(2,0) * diff[3] + arg(3,0) * diff[4];
		tM(2,1) =-arg(0,0) * diff[1] + arg(1,0) * diff[3] - arg(3,0) * diff[5];
		tM(3,1) = arg(0,0) * diff[2] - arg(1,0) * diff[4] + arg(2,0) * diff[5];

		
		//calculate second 8 elements
		pairs[0] = arg(2,0) * arg(3,1);
		pairs[1] = arg(2,1) * arg(3,0);
		pairs[2] = arg(1,0) * arg(3,1);
		pairs[3] = arg(1,1) * arg(3,0);
		pairs[4] = arg(1,0) * arg(2,1);
		pairs[5] = arg(2,0) * arg(1,1);

		diff[0] = pairs[0] - pairs[1];
		diff[1] = pairs[2] - pairs[3];
		diff[2] = pairs[4] - pairs[5];

		pairs[0] = arg(0,0) * arg(3,1);
		pairs[1] = arg(0,1) * arg(3,0);
		pairs[2] = arg(0,0) * arg(2,1);
		pairs[3] = arg(2,0) * arg(0,1);
		pairs[4] = arg(0,0) * arg(1,1);
		pairs[5] = arg(1,0) * arg(0,1);

		diff[3] = pairs[0] - pairs[1];
		diff[4] = pairs[2] - pairs[3];
		diff[5] = pairs[4] - pairs[5];

		tM(0,2) = arg(1,3) * diff[0] - arg(2,3) * diff[1] + arg(3,3) * diff[2];
		tM(1,2) =-arg(0,3) * diff[0] + arg(2,3) * diff[3] - arg(3,3) * diff[4];
		tM(2,2) = arg(0,3) * diff[1] - arg(1,3) * diff[3] + arg(3,3) * diff[5];
		tM(3,2) =-arg(0,3) * diff[2] + arg(1,3) * diff[4] - arg(2,3) * diff[5];

		tM(0,3) =-arg(1,2) * diff[0] + arg(2,2) * diff[1] - arg(3,2) * diff[2];
		tM(1,3) = arg(0,2) * diff[0] - arg(2,2) * diff[3] + arg(3,2) * diff[4];
		tM(2,3) =-arg(0,2) * diff[1] + arg(1,2) * diff[3] - arg(3,2) * diff[5];
		tM(3,3) = arg(0,2) * diff[2] - arg(1,2) * diff[4] + arg(2,2) * diff[5];


		det = tM(0,0) * arg(0,0) + tM(0,1) * arg(0,1) + tM(0,2) * arg(0,2) + tM(0,3) * arg(0,3);

		if(det)
		{
			resdet = det;
			det = 1.0f / det;
		}
		else 
			return det;

		arg(0,0) = tM(0,0) * det;
		arg(1,0) = tM(0,1) * det;
		arg(2,0) = tM(0,2) * det;
		arg(3,0) = tM(0,3) * det;

		arg(0,1) = tM(1,0) * det;
		arg(1,1) = tM(1,1) * det;
		arg(2,1) = tM(1,2) * det;
		arg(3,1) = tM(1,3) * det;

		arg(0,2) = tM(2,0) * det;
		arg(1,2) = tM(2,1) * det;
		arg(2,2)= tM(2,2) * det;
		arg(3,2)= tM(2,3) * det;

		arg(0,3) = tM(3,0) * det;
		arg(1,3) = tM(3,1) * det;
		arg(2,3) = tM(3,2) * det;
		arg(3,3) = tM(3,3) * det;

		return resdet;
};
//inverse 3x3 matrix
template< class T, class TagT >
THOR_INLINE T
InverseImpl( ThFixedMatrix<T,3,3,TagT>& arg )
{
	typedef ThFixedMatrix<T,3,3,TagT> mat_t;
	typedef typename mat_t::value_type value_type;
	value_type det, retdet;
	mat_t temp;

	temp(0,0) = arg(1,1) * arg(2,2) - arg(1,2) * arg(2,1);
	temp(0,1) = arg(1,2) * arg(2,0) - arg(1,0) * arg(2,2);
	temp(0,2) = arg(1,0) * arg(2,1) - arg(1,1) * arg(2,0);
	temp(1,0) = arg(0,2) * arg(2,1) - arg(0,1) * arg(2,2);
	temp(1,1) = arg(0,0) * arg(2,2) - arg(0,2) * arg(2,0);
	temp(1,2) = arg(0,1) * arg(2,0) - arg(0,0) * arg(2,1);
	temp(2,0) = arg(0,1) * arg(1,2) - arg(1,1) * arg(0,2);
	temp(2,1) = arg(0,2) * arg(1,0) - arg(0,0) * arg(1,2);
	temp(2,2) = arg(1,1) * arg(0,0) - arg(1,0) * arg(0,1);

	det = arg(0,0) * temp(0,0) + arg(0,1) * temp(0,1) + arg(0,2) * temp(0,2) ;
	
	if(det)
	{
		retdet = det;
		det = 1 / det;
	}
	else
		return det;


	arg(0,0) = temp(0,0) * det;  arg(1,0) = temp(1,0) * det;  arg(2,0) = temp(2,0) * det;   
	arg(0,1) = temp(0,1) * det;  arg(1,1) = temp(1,1) * det;  arg(2,1) = temp(2,1) * det;   
	arg(0,2) = temp(0,2) * det;  arg(1,2) = temp(1,2) * det;  arg(2,2) = temp(2,2) * det; 

	return retdet;
};

//inverse 2x2 matrix
template< class T, class TagT >
THOR_INLINE T
InverseImpl( ThFixedMatrix<T,2,2,TagT>& arg )
{
	typedef ThFixedMatrix<T,2,2,TagT> mat_t;
	typedef typename mat_t::value_type value_type;
	value_type det, retdet;

	det = arg(0,0) * arg(1,1) - arg(0,1) * arg(1,0);
	
	if(det)
	{
		retdet = det;
		det = 1 / det;
	}
	else
		return det;

	arg(0,0) = arg(1,1) * det;   arg(0,1) = -arg(0,1) * det;   
	arg(1,0) = -arg(1,0) * det;  arg(1,1) = arg(1,1) * det;	

	return retdet;
};

template< class T, class TagT, class RealT >
THOR_INLINE void
PerspectiveImpl( ThFixedMatrix<T,4,4,TagT>& arg, RealT fovy, RealT aspect, RealT zNear, RealT zFar )
{
	typedef typename ThFixedMatrix<T,4,4,TagT>::value_type value_type;
	value_type f=Math::Cotan( fovy / value_type(2.0) );
	/*value_type inv = value_type(1.0) / (zNear-zFar);

	arg(0,0) = f / aspect;		arg(0,1) = value_type(0.0);	arg(0,2) = value_type(0.0);		arg(0,3) = value_type(0.0);
	arg(1,0) = value_type(0.0);	arg(1,1) = f;				arg(1,2) = value_type(0.0);		arg(1,3) = value_type(0.0);
	arg(2,0) = value_type(0.0);	arg(2,1) = value_type(0.0);	arg(2,2)=(zFar + zNear) * inv;	arg(2,3) = value_type(2.0) * zFar * zNear * inv;
	arg(3,0) = value_type(0.0);	arg(3,1) = value_type(0.0);	arg(3,2)=value_type(-1.0);		arg(3,3) = value_type(0.0);*/

	value_type inv = value_type(1.0) / (zNear - zFar);

	arg(0,0) = f / aspect;		arg(0,1) = value_type(0.0);	arg(0,2) = value_type(0.0);			arg(0,3) = value_type(0.0);
	arg(1,0) = value_type(0.0);	arg(1,1) = f;				arg(1,2) = value_type(0.0);			arg(1,3) = value_type(0.0);
	arg(2,0) = value_type(0.0);	arg(2,1) = value_type(0.0);	arg(2,2) = (zFar + zNear) * inv;	arg(2,3) = value_type(2.0) * zFar * zNear * inv;
	arg(3,0) = value_type(0.0);	arg(3,1) = value_type(0.0);	arg(3,2) = -value_type(1.0);		arg(3,3) = value_type(0.0);
};

template< class T, class TagT, class RealT >
THOR_INLINE void
OrthoImpl( ThFixedMatrix<T,4,4,TagT>& arg, RealT left, RealT right, RealT bottom, RealT top, RealT zNear, RealT zFar )
{
	typedef typename ThFixedMatrix<T,4,4,TagT>::value_type value_type;
	value_type rl = value_type(1.0) / (right - left);
	value_type tb = value_type(1.0) / (top - bottom);
	value_type fn = value_type(1.0) / (zFar - zNear);

	arg(0,0) = value_type(2.0) * rl;	arg(0,1) = value_type(0.0);			arg(0,2) = value_type(0.0);			arg(0,3) = -(right + left) * rl;
	arg(1,0) = value_type(0.0);			arg(1,1) = value_type(2.0) * tb;	arg(1,2) = value_type(0.0);			arg(1,3) = -(top + bottom) * tb;
	arg(2,0) = value_type(0.0);			arg(2,1) = value_type(0.0);			arg(2,2) = value_type(-2.0) * fn;	arg(2,3) = -(zFar + zNear) * fn;
	arg(3,0) = value_type(0.0);			arg(3,1) = value_type(0.0);			arg(3,2) = value_type(0.0);			arg(3,3) = value_type(1.0);
};

template< class T, class TagT, class RealT >
THOR_INLINE void
PerspectiveDXImpl( ThFixedMatrix<T,4,4,TagT>& arg, RealT fovy, RealT aspect, RealT zNear, RealT zFar )
{
	typedef typename ThFixedMatrix<T,4,4,TagT>::value_type value_type;

	value_type yScale = Math::Cotan( fovy / value_type(2.0) );
	value_type xScale = yScale / aspect;
	value_type inv = value_type(1.0) / (zNear-zFar);

	//perspective
	arg(0,0) = xScale;				arg(0,1) = value_type(0.0);		arg(0,2) = value_type(0.0);		arg(0,3) = value_type(0.0);
	arg(1,0) = value_type(0.0);		arg(1,1) = yScale;				arg(1,2) = value_type(0.0);		arg(1,3) = value_type(0.0);
	arg(2,0) = value_type(0.0);		arg(2,1) = value_type(0.0);		arg(2,2) = zFar * inv;			arg(2,3) = zNear * zFar * inv;
	arg(3,0) = value_type(0.0);		arg(3,1) = value_type(0.0);		arg(3,2) = -value_type(1.0);	arg(3,3) = value_type(0.0);
};

template< class T, class TagT, class RealT >
THOR_INLINE void
OrthoDXImpl( ThFixedMatrix<T,4,4,TagT>& arg, RealT left, RealT right, RealT bottom, RealT top, RealT zNear, RealT zFar )
{
	typedef typename ThFixedMatrix<T,4,4,TagT>::value_type value_type;
	value_type rl = value_type(1.0) / (right - left);
	value_type tb = value_type(1.0) / (top - bottom);
	value_type nf = value_type(1.0) / (zNear - zFar);

	arg(0,0) = value_type(2.0) * rl;	arg(0,1) = value_type(0.0);			arg(0,2) = value_type(0.0);			arg(0,3) = -(left + right) * rl;
	arg(1,0) = value_type(0.0);			arg(1,1) = value_type(2.0) * tb;	arg(1,2) = value_type(0.0);			arg(1,3) = -(top + bottom) * tb;
	arg(2,0) = value_type(0.0);			arg(2,1) = value_type(0.0);			arg(2,2) = value_type(1.0) * nf;	arg(2,3) = zNear * nf;
	arg(3,0) = value_type(0.0);			arg(3,1) = value_type(0.0);			arg(3,2) = value_type(0.0);			arg(3,3) = value_type(1.0);

};

template< class T, class TagT, class VecT >
THOR_INLINE void
LookAtImpl( ThFixedMatrix<T,4,4,TagT>& arg, const VecT& eye_ , const VecT& center_ , const VecT& up_ )
{
	typedef typename ThFixedMatrix<T,4,4,TagT>::value_type value_type;
	VecT eye = center_ - eye_;//f
	eye.Normalize();
	VecT up(up_);
	up.Normalize();//up`
	VecT center = eye % up;//s
	up = center % eye;//u

	arg(0,0) = center.x();		arg(0,1) = center.y();		arg(0,2) = center.z();		arg(0,3) = -eye_(0);
	arg(1,0) = up.x();			arg(1,1) = up.y();			arg(1,2) = up.z();			arg(1,3) = -eye_(1);
	arg(2,0) = -eye.x();		arg(2,1) = -eye.y();		arg(2,2) = -eye.z();		arg(2,3) = -eye_(2);
	arg(3,0) = value_type(0.0);	arg(3,1) = value_type(0.0);	arg(3,2) = value_type(0.0);	arg(3,3) = value_type(1.0);
}

template< class T, class TagT, class VecT >
THOR_INLINE void
TranslateImpl( ThFixedMatrix<T,4,4,TagT>& arg, const VecT& t )
{
	typedef typename ThFixedMatrix<T,4,4,TagT>::value_type value_type;

	arg(0,0) = value_type(1.0);	arg(0,1) = value_type(0.0);	arg(0,2) = value_type(0.0);	arg(0,3) = t.x();
	arg(1,0) = value_type(0.0);	arg(1,1) = value_type(1.0);	arg(1,2) = value_type(0.0);	arg(1,3) = t.y();
	arg(2,0) = value_type(0.0);	arg(2,1) = value_type(0.0);	arg(2,2) = value_type(1.0);	arg(2,3) = t.z();
	arg(3,0) = value_type(0.0);	arg(3,1) = value_type(0.0);	arg(3,2) = value_type(0.0);	arg(3,3) = value_type(1.0);
}

template< class MatT, class VecT >
THOR_INLINE void
ScaleImpl( MatT& arg, const VecT& s )
{
	//typedef typename FixedMatrix<T,4,4,TagT>::value_type value_type;	

	LoadIdentityImpl( arg );

	arg(0,0) = s.x();				
					arg(1,1) = s.y();
									arg(2,2) = s.z();	
}

template< class MatT, class RealT >
THOR_INLINE void 
RotateXImpl( MatT& arg, RealT angle )
{
	typedef typename MatT::value_type value_type;	

	value_type s, c;
	Math::SinCos( Math::DegToRad(angle), s, c);

	LoadIdentityImpl( arg );

	//arg(0,0) = value_type(1.0);	arg(0,1) = value_type(0.0);	arg(0,2) = value_type(0.0);	arg(0,3) = value_type(0.0);
	/*arg(1,0) = value_type(0.0);*/	arg(1,1) = c;				arg(1,2) = -s;				/*arg(1,3) = value_type(0.0);*/
	/*arg(2,0) = value_type(0.0);*/	arg(2,1) = s;				arg(2,2) = c;				/*arg(2,3) = value_type(0.0);*/
	//arg(3,0) = value_type(0.0);	arg(3,1) = value_type(0.0);	arg(3,2) = value_type(0.0);	arg(3,3) = value_type(1.0);	
}

template< class MatT, class RealT >
THOR_INLINE void
RotateYImpl ( MatT& arg, RealT angle )
{
	typedef typename MatT::value_type value_type;	

	value_type s,c;
	Math::SinCos( Math::DegToRad(angle), s, c);

	LoadIdentityImpl( arg );

	arg(0,0) = c;					/*arg(0,1) = value_type(0.0);*/	arg(0,2) = s;				//arg(0,3) = value_type(0.0);
	//arg(1,0) = value_type(0.0);	arg(1,1) = value_type(1.0);		arg(1,2) = value_type(0.0);	arg(1,3) = value_type(0.0);
	arg(2,0) = -s;					/*arg(2,1) = value_type(0.0);*/	arg(2,2) = c;				//arg(2,3) = value_type(0.0);
	//arg(3,0) = value_type(0.0);	arg(3,1) = value_type(0.0);		arg(3,2) = value_type(0.0);	arg(3,3) = value_type(1.0);	
}

template< class MatT, class RealT >
THOR_INLINE void
RotateZImpl ( MatT& arg, RealT angle )
{
	typedef typename MatT::value_type value_type;	

	value_type s,c;
	Math::SinCos( Math::DegToRad(angle), s, c);

	LoadIdentityImpl( arg );

	arg(0,0) = c;					arg(0,1) = -s;				//arg(0,2) = value_type(0.0);	arg(0,3) = value_type(0.0);
	arg(1,0) = s;					arg(1,1) = c;				//arg(1,2) = value_type(0.0);	arg(1,3) = value_type(0.0);
	//arg(2,0) = value_type(0.0);	arg(2,1) = value_type(0.0);	arg(2,2) = value_type(1.0);		arg(2,3) = value_type(0.0);
	//arg(3,0) = value_type(0.0);	arg(3,1) = value_type(0.0);	arg(3,2) = value_type(0.0);		arg(3,3) = value_type(1.0);	
}

template< class MatT, class RealT, class VecT >
THOR_INLINE void
RotateImpl ( MatT& arg, RealT angle, const VecT& axis_ )
{
	typedef typename MatT::value_type value_type;	
	
	value_type s,c;
	Math::SinCos( Math::DegToRad(angle), s, c);
	VecT axis(axis_);
	axis.Normalize();
	
	value_type	c1 = 1 - c, xy = axis.x() * axis.y(), xz = axis.x() * axis.z(),
				yz = axis.y() * axis.z(), zs = axis.z() * s,
				ys = axis.y() * s, xs = axis.x() * s;

	LoadIdentityImpl( arg );
	
	arg(0,0) = c1 * axis.x() * axis.x() + c;	arg(0,1) = c1 * xy - zs;					arg(0,2) = c1 * xz + ys;					//arg(0,3) = value_type(0.0);
	arg(1,0) = c1 * xy + zs;					arg(1,1) = c1 * axis.y() * axis.y() + c;	arg(1,2) = c1 * yz - xs;					//arg(1,3) = value_type(0.0);
	arg(2,0) = c1 * xz - ys;					arg(2,1) = c1 * yz + xs;					arg(2,2) = c1 * axis.z() * axis.z() + c;	//arg(2,3) = value_type(0.0);
	//arg(3,0) = value_type(0.0);				arg(3,1) = value_type(0.0);					arg(3,2) = value_type(0.0);					arg(3,3) = value_type(1.0);
}
