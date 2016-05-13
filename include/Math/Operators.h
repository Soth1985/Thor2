#ifndef THOR_OPERATORS_H
#define THOR_OPERATORS_H

#include  <Thor/Framework/Common.h>
#include  <Thor/ThorMath/ExpressionNodes.h>
#include  <Thor/ThorMath/OpTags.h>


namespace Thor{

template<>
struct unary_result< ThVecUnaryMinusTag,ThFixedVector<float,4> >
{
	typedef ThFixedVector<float,4> result_t;
};

template<>
struct binary_result< ThVecBinaryPlusTag,ThFixedVector<float, 4>, ThFixedVector<float, 4> >
{
	typedef ThFixedVector<float, 4> result_t;
};


//unary minus
THOR_INLINE MakeNode< ThVecUnaryMinusTag, ThFixedVector<float, 4> >::expr_t
//UnaryNode<VecUnaryMinusTag, FixedVector<float,4> >
operator-(const ThFixedVector<float, 4>& arg)
{
	typedef MakeNode< ThVecUnaryMinusTag, ThFixedVector<float, 4> >::node_t node_t;
	typedef MakeNode< ThVecUnaryMinusTag, ThFixedVector<float, 4> >::expr_t expr_t;
	//return UnaryNode<VecUnaryMinusTag, FixedVector<float, 4> > (arg);
	return expr_t( node_t( Leaf< ThFixedVector<float, 4> >(arg) ) );
};

template< class T, class R >
THOR_INLINE typename MakeNode< ThVecUnaryMinusTag, Expression<T,R> >::expr_t
operator-(const Expression<T,R>& arg)
{
	typedef typename MakeNode< ThVecUnaryMinusTag, Expression<T,R> >::node_t node_t;
	typedef typename MakeNode< ThVecUnaryMinusTag, Expression<T,R> >::expr_t expr_t;

	return expr_t( node_t( arg.tree ) );
};
//binary plus
THOR_INLINE MakeNode< ThVecBinaryPlusTag, ThFixedVector<float, 4>, ThFixedVector<float, 4> >::expr_t
operator+(const ThFixedVector<float, 4>& lhs, const ThFixedVector<float, 4>& rhs)
{
	typedef MakeNode< ThVecBinaryPlusTag, ThFixedVector<float, 4>, ThFixedVector<float, 4> >::node_t node_t;
	typedef MakeNode< ThVecBinaryPlusTag, ThFixedVector<float, 4>, ThFixedVector<float, 4> >::expr_t expr_t;

	return expr_t( node_t( Leaf< ThFixedVector<float, 4> >(lhs), Leaf< ThFixedVector<float, 4> >(rhs) ) );
};
//
template < class T1, class R1 >
THOR_INLINE typename MakeNode< ThVecBinaryPlusTag, Expression<T1,R1>, ThFixedVector<float,4> >::expr_t
operator+(const Expression<T1,R1>& lhs, const ThFixedVector<float,4>& rhs)
{
	typedef MakeNode< ThVecBinaryPlusTag, Expression<T1,R1>, ThFixedVector<float, 4> >::node_t node_t;
	typedef MakeNode< ThVecBinaryPlusTag, Expression<T1,R1>, ThFixedVector<float, 4> >::expr_t expr_t;

	return expr_t( node_t( lhs.tree, Leaf< ThFixedVector<float, 4> >(rhs) ) );
};
//
template < class T2, class R2 >
THOR_INLINE typename MakeNode< ThVecBinaryPlusTag, ThFixedVector<float,4>, Expression<T2,R2> >::expr_t
operator+(const ThFixedVector<float,4>& lhs, const Expression<T2,R2>& rhs)
{
	typedef MakeNode< ThVecBinaryPlusTag, ThFixedVector<float, 4>, Expression<T2,R2> >::node_t node_t;
	typedef MakeNode< ThVecBinaryPlusTag, ThFixedVector<float, 4>, Expression<T2,R2> >::expr_t expr_t;

	return expr_t( node_t( Leaf< ThFixedVector<float, 4> >(lhs), rhs.tree ) );
};
//
template < class T1, class T2, class R1, class R2 >
THOR_INLINE typename MakeNode< ThVecBinaryPlusTag, Expression<T1,R1>, Expression<T2,R2> >::expr_t
operator+(const Expression<T1,R1>& lhs, const Expression<T2,R2>& rhs)
{
	typedef MakeNode< ThVecBinaryPlusTag, Expression<T1,R1>, Expression<T2,R2> >::node_t node_t;
	typedef MakeNode< ThVecBinaryPlusTag, Expression<T1,R1>, Expression<T2,R2> >::expr_t expr_t;

	return expr_t( node_t( lhs.tree, rhs.tree ) );
};

}//Thor

#endif