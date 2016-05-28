template< class VecT >
THOR_INLINE void VecUnaryMinus( const VecT& arg, VecT& res )
{
	for ( unsigned int i = 0; i < arg.Size(); ++i )
		res(i) = -arg(i);
};

template< class VecT >
THOR_INLINE void VecNorm2( const VecT& arg, VecT& res )
{
	typedef typename VecT::value_type value_type;
	value_type len = arg.Length();

	if( len == value_type(0.0) ) 
		return;

	len = value_type(1.0) / len;

	for( unsigned int i = 0; i < arg.Size(); ++i )
		res(i) = arg(i) * len;
};


template< class VecT1, class VecT2, class ResT >
THOR_INLINE void VecModulus( const VecT1& lhs, const VecT2& rhs, ResT& res )
{};

template< class VecT1, class VecT2, class ResT >
THOR_INLINE void VecXor( const VecT1& lhs, const VecT2& rhs, ResT& res )
{};

//cross product of two 4dim vectors
template< class T1, class TagT1, class AllocT1, class T2, class TagT2, class AllocT2, class ResT >
THOR_INLINE void VecModulus( const ThFixedVector<T1,4,TagT1,AllocT1>& lhs, const ThFixedVector<T2,4,TagT2,AllocT2>& rhs, ResT& res )
{
	typedef typename ResT::value_type value_type;
	res(0) = lhs.y() * rhs.z() - lhs.z() * rhs.y();
	res(1) = lhs.z() * rhs.x() - lhs.x() * rhs.z();
	res(2) = lhs.x() * rhs.y() - lhs.y() * rhs.x();
	res(3) = value_type(0.0);		
};

//cross product of two 3dim vectors
template< class T1, class TagT1, class AllocT1, class T2, class TagT2, class AllocT2, class ResT >
THOR_INLINE void VecModulus( const ThFixedVector<T1,3,TagT1,AllocT1>& lhs, const ThFixedVector<T2,3,TagT2,AllocT2>& rhs, ResT& res )
{
	res(0) = lhs.y() * rhs.z() - lhs.z() * rhs.y();
	res(1) = lhs.z() * rhs.x() - lhs.x() * rhs.z();
	res(2) = lhs.x() * rhs.y() - lhs.y() * rhs.x();
};

//adds two generic vectors
template< class VecT1, class VecT2, class ResT >
THOR_INLINE void VecAdd( const VecT1& lhs, const VecT2& rhs, ResT& res )
{
	for ( unsigned int i = 0; i < lhs.Size(); ++i )
		res(i) = lhs(i) + rhs(i);
};

//checks for equality of two generic vectors
template< class VecT1, class VecT2 >
THOR_INLINE bool AreVecEqual( const VecT1& lhs, const VecT2& rhs )
{
	for ( unsigned int i = 0; i < lhs.Size(); ++i )
	{
		if ( Math::Abs( lhs(i) - rhs(i) ) > Math::E4 )
			return false;
	}

	return true;
};


//substracts rhs vector from lhs
template< class VecT1, class VecT2, class ResT >
THOR_INLINE void VecSub( const VecT1& lhs, const VecT2& rhs, ResT& res )
{
	for ( unsigned int i = 0; i < lhs.Size(); ++i )
		res(i) = lhs(i) - rhs(i);
};

//calcs dot product of lhs and rhs
template< class VecT1, class VecT2, class RealT >
THOR_INLINE void VecDot( const VecT1& lhs, const VecT2& rhs, RealT& res )
{
	res = 0.0;
	for ( unsigned int i = 0; i < lhs.Size(); ++i )
		res += lhs(i) * rhs(i);
};

//multilplies vector lhs by a scalar rhs
template< class VecT, class ResT, class RealT >
THOR_INLINE void VecMulScalar( const VecT& lhs, const RealT& rhs, ResT& res )
{
	for ( unsigned int i = 0; i < lhs.Size(); ++i )
		res(i) = lhs(i) * rhs;
};

//divides vector lhs by a scalar rhs
template< class VecT, class ResT, class RealT >
THOR_INLINE void VecDivScalar( const VecT& lhs, const RealT& rhs, ResT& res )
{
	for ( unsigned int i = 0; i < lhs.Size(); ++i )
		res(i) = lhs(i) / rhs;
};

//assigns lhs vector to rhs
template< class VecT1, class VecT2 >
THOR_INLINE void VecAssign( VecT1& lhs, const VecT2& rhs )
{
	for ( unsigned int i = 0; i < lhs.Size(); ++i )
		lhs(i) = rhs(i);
};

//assigns lhs to rhs
template< class LhsT, class RealT >
THOR_INLINE void VecAssignScalar( LhsT& lhs, const RealT& rhs )
{
	for ( unsigned int i = 0; i < lhs.Size(); ++i )
		lhs(i) = rhs;
};

#ifndef USE_VECTOR_EXPRESSION_TEMPLATES
////////////////////////////////////////////////////////////////////////////////
/*template < class VecT >
THOR_INLINE VecT Normalize( VecT &v)
{
	VecT tmp;
	VecNorm2(v,tmp);
	return tmp;
};*/

#endif