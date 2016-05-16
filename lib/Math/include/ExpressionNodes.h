#pragma once

#include  <Thor/Framework/Common.h>

namespace Thor{
///////////////////////////////////////////////////////////////////
/**	   
*	nil_t represents no type template parameter.
*/
struct nil_t
{};


template < class OpT >
struct Traverser;
///////////////////////////////////////////////////////////////////
/**	   
*	This class represents binary operations in the expression parse tree.
*	The temporary type TempT for a given expression is considered the same for all parts of expression, 
*	and is equal to the type of Block of the object the expression is assigned to.
*	OpT should have a "static apply( const LhsT& l, const RhsT& r, TempT& tmp, const IterT& i )" method.
*	This expression template expects a single temporary Block as input per expression.
*/
template < class OpT, class LhsT, class RhsT  >
struct BinaryNode
{
//in debug mode we should store values instead of references to avoid crushes 
//#ifdef _DEBUG
//	const LhsT& lhs;													
//	const RhsT& rhs;
//#else
	const LhsT lhs;													
	const RhsT rhs;		
//#endif

	typedef OpT op_t;

	typedef LhsT lhs_t;
	typedef RhsT rhs_t;
	typedef BinaryNode< OpT, LhsT, RhsT > self_t;

	//typedef TempT temp_t;

	/*unsigned int block_size()
	{
		return block_sz;
	};*/	

	template < class TempT, class IterT >
	THOR_INLINE void traverse( TempT& tmp, const IterT& i ) const
	{		
		//#pragma inline_depth(2048)
		Traverser<OpT>::visit_node( lhs, rhs, tmp, i );		
	};

	template < class ReturnT, class IterT  >
	THOR_INLINE ReturnT at( const IterT& i ) const
	{
		//return OpT::apply< ReturnT >( lhs.at<ReturnT>(i), rhs.at<ReturnT>(i) );
		return OpT::apply< ReturnT >( lhs, rhs, i );

	};

	template < class ReturnT, class IterT  >
	THOR_INLINE ReturnT at( const IterT& i, const IterT& j ) const
	{
		//return OpT::apply< ReturnT >( lhs.at<ReturnT>(i), rhs.at<ReturnT>(i) );
		return OpT::apply< ReturnT >( lhs, rhs, i, j );

	};	

	THOR_INLINE BinaryNode( const LhsT& l, const RhsT& r ): lhs(l), rhs(r) {};	
	THOR_INLINE BinaryNode( const BinaryNode& copy ): lhs(copy.lhs), rhs(copy.rhs) {};
	BinaryNode& operator=( const BinaryNode& ){};

};
///////////////////////////////////////////////////////////////////
/**	   
*	This class represents unary operations in the expression parse tree.
*	The temporary type TempT for a given expression is considered the same for all parts of expression, 
*	and is equal to the type of Block of the object the expression is assigned to.
*	OpT should have a "static apply( const ArgT& a, TempT& tmp, const IterT& i )" method.
*	This expression template expects a single temporary Block as input per expression.
*/
template < class OpT, class ArgT >
struct UnaryNode
{
//in debug mode we should store values instead of references to avoid crushes 
//#ifdef _DEBUG
//	const ArgT arg;
//#else
	const ArgT arg;		
//#endif

	typedef OpT op_t;

	typedef ArgT arg_t;
	typedef UnaryNode< OpT, ArgT > self_t;


	template < class TempT, class IterT >
	THOR_INLINE void traverse( TempT& tmp, const IterT& i ) const
	{

		Traverser<OpT>::visit_node( arg, tmp, i );		
	};	

	//template < class IterT >
	/*THOR_INLINE const float operator[]( int& i ) const
	{		
		
		OpT::apply( arg, tmp, i );		
	};	*/

	template < class ReturnT, class IterT >
	THOR_INLINE  ReturnT at( const IterT& i ) const
	{
		//return OpT::apply<ReturnT>( arg.at<ReturnT>(i) );
		return OpT::apply< ReturnT >( arg, i );
		
	};

	template < class ReturnT, class IterT >
	THOR_INLINE  ReturnT at( const IterT& i, const IterT& j ) const
	{
		//return OpT::apply<ReturnT>( arg.at<ReturnT>(i) );
		return OpT::apply< ReturnT >( arg, i, j );
		
	};

	THOR_INLINE UnaryNode( const ArgT& a ): arg(a){};
	THOR_INLINE UnaryNode( const UnaryNode& copy ): arg( copy.arg ){};
	UnaryNode& operator=( const UnaryNode& ){};

};

template < class DataT >
class Leaf;

///////////////////////////////////////////////////////////////////
/**	   
*	Expression stores the parse tree.
*/
template < class ExprT, class ResultT >
struct Expression
{
	typedef ExprT node_t;
	typedef ResultT result_t;
	const ExprT tree;
	
	template < class TempT, class IterT >
	THOR_INLINE void traverse( TempT& tmp, const IterT& i ) const
	{		
		tree.traverse(  tmp, i );		
	};	
	
	THOR_INLINE Expression( const ExprT& node ):tree(node){};
	THOR_INLINE Expression( const Expression& copy ):tree(copy.tree){};
	Expression& operator=( const Expression& ){};
};




template <class OpT, class T>
struct unary_result;

template <class OpT, class L, class R>
struct binary_result;
///////////////////////////////////////////////////////////////////
/**	   
*	MakeNode creates a parse tree node and plugs it into the tree.
*	node_t is the type of a new node,
*	expr_t is a resulting tree after a new node insertion.
*   OpT is operator tag,
*	T1 and T2 are node children,
*	T3 is dummy parameter.
*/
//Generic case
template < class OpT = nil_t, class T1 = nil_t, class T2 = nil_t, class T3 = nil_t >
struct MakeNode
{
	typedef nil_t node_t;
	typedef Expression< nil_t, nil_t > expr_t;
};
//Leaf unary node
template < class OpT, class T1 >
struct MakeNode< OpT, T1 >
{
	typedef UnaryNode< OpT, Leaf<T1> > node_t;
	typedef typename unary_result<OpT,T1>::result_t result_t;
	typedef Expression< node_t, result_t > expr_t;
};
//Unary node
template < class OpT, class T1, class R >
struct MakeNode< OpT, Expression<T1,R> >
{
	typedef UnaryNode< OpT, T1 > node_t;
	typedef typename Expression<T1,R>::result_t prev_result_t;
	typedef typename unary_result<OpT,prev_result_t>::result_t result_t;
	typedef Expression< node_t, result_t > expr_t;
};
//Leaf binary node
template < class OpT, class T1, class T2 >
struct MakeNode< OpT, T1, T2 >
{
	typedef BinaryNode< OpT, Leaf<T1>, Leaf<T2> > node_t;
	typedef typename binary_result<OpT,T1, T2>::result_t result_t;
	typedef Expression< node_t, result_t > expr_t;
};
//Binary node with left leaf
template < class OpT, class T1, class T2, class R1 >
struct MakeNode< OpT, Expression<T1,R1>, T2 >
{
	typedef BinaryNode< OpT, T1, Leaf<T2> > node_t;
	typedef typename Expression<T1,R1>::result_t left_result_t;
	typedef typename binary_result<OpT,left_result_t,T2>::result_t result_t;
	typedef Expression< node_t, result_t > expr_t;
};
//Binary node with right leaf
template < class OpT, class T1, class T2, class R2 >
struct MakeNode< OpT, T1, Expression<T2,R2> >
{
	typedef BinaryNode< OpT, Leaf<T1>, T2 > node_t;
	typedef typename Expression<T2,R2>::result_t right_result_t;
	typedef typename binary_result<OpT,T1,right_result_t>::result_t result_t;
	typedef Expression< node_t, result_t > expr_t;
};
//Binary node
template < class OpT, class T1, class T2, class R1, class R2 >
struct MakeNode< OpT, Expression<T1,R1>, Expression<T2,R2> >
{
	typedef BinaryNode< OpT, T1, T2 > node_t;
	typedef typename Expression<T1,R1>::result_t left_result_t;
	typedef typename Expression<T2,R2>::result_t right_result_t;
	typedef typename binary_result<OpT,left_result_t,right_result_t>::result_t result_t;
	typedef Expression< node_t, result_t > expr_t;
};

template < class DataT >
class Leaf
{
	const DataT& data;
public:
	
	template < class ReturnT, class IterT >
	THOR_INLINE const ReturnT& at( const IterT& i ) const
	{	
		return data(i);
	};

	template < class ReturnT, class IterT1, class IterT2 >
	THOR_INLINE const ReturnT& at( const IterT1& i, const IterT2& j ) const
	{	
		return data(i,j);
	};

	//reference accessor
	THOR_INLINE const DataT& ref() const
	{	
		return data;
	};

	THOR_INLINE Leaf( const DataT& data_ ): data(data_){};
	THOR_INLINE Leaf( const Leaf& copy ): data(copy.data){};
	Leaf& operator=( const Leaf& copy ){};
};
//
template <>
class Leaf<float>
{
	const float data;
public:
	//vector accessor
	template < class ReturnT, class IterT >
	THOR_INLINE const ReturnT& at( const IterT& i ) const
	{	
		return data;
	};

	template < class ReturnT, class IterT >
	THOR_INLINE const ReturnT& at( const IterT& i, const IterT& j ) const
	{	
		return data;
	};

	//reference accessor
	THOR_INLINE const float& ref(void) const
	{	
		return data;
	};

	THOR_INLINE Leaf( const float& data_ ): data(data_){};
	THOR_INLINE Leaf( const Leaf& copy ): data(copy.data){};
	Leaf& operator=( const Leaf&  ){};
};
//
template <>
class Leaf<double>
{
	const double data;
public:
	//vector accessor
	template < class ReturnT, class IterT >
	THOR_INLINE const ReturnT& at( const IterT& i ) const
	{	
		return data;
	};

	template < class ReturnT, class IterT >
	THOR_INLINE const ReturnT& at( const IterT& i, const IterT& j ) const
	{	
		return data;
	};

	//reference accessor
	THOR_INLINE const double& ref() const
	{	
		return data;
	};

	THOR_INLINE Leaf( const double& data_ ): data(data_){};
	THOR_INLINE Leaf( const Leaf& copy ): data(copy.data){};
	Leaf& operator=( const Leaf&  ){};
};
//
template <>
class Leaf<int>
{
	const int data;
public:
	
	template < class ReturnT, class IterT >
	THOR_INLINE const ReturnT& at( const IterT& i ) const
	{	
		return data;
	};

	template < class ReturnT, class IterT >
	THOR_INLINE const ReturnT& at( const IterT& i, const IterT& j ) const
	{	
		return data;
	};

	//reference accessor
	THOR_INLINE const int& ref() const
	{	
		return data;
	};

	THOR_INLINE Leaf( const int& data_ ): data(data_){};
	THOR_INLINE Leaf( const Leaf& copy ): data(copy.data){};
	Leaf& operator=( const Leaf&  ){};
};
//
template <>
class Leaf<unsigned int>
{
	const unsigned int data;
public:
	
	template < class ReturnT, class IterT >
	THOR_INLINE const ReturnT& at( const IterT &i ) const
	{	
		return data;
	};

	template < class ReturnT, class IterT >
	THOR_INLINE const ReturnT& at( const IterT& i, const IterT& j ) const
	{	
		return data;
	};

	//reference accessor
	THOR_INLINE const unsigned int& ref() const
	{	
		return data;
	};

	THOR_INLINE Leaf( const unsigned int& data_ ): data(data_){};
	THOR_INLINE Leaf( const Leaf& copy ): data(copy.data){};
	Leaf& operator=( const Leaf&  ){};
};
//
template <>
class Leaf<char>
{
	const char data;
public:
	
	template < class ReturnT, class IterT >
	THOR_INLINE const ReturnT& at( const IterT& i ) const
	{	
		return data;
	};

	template < class ReturnT, class IterT >
	THOR_INLINE const ReturnT& at( const IterT& i, const IterT& j ) const
	{	
		return data;
	};

	//reference accessor
	THOR_INLINE const char& ref() const
	{	
		return data;
	};

	THOR_INLINE Leaf( const char& data_ ): data(data_){};
	THOR_INLINE Leaf( const Leaf& copy ): data(copy.data){};
	Leaf& operator=( const Leaf&  ){};
};


}//Thor