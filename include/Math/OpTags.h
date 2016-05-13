#pragma once

#include  <Thor/Framework/Common.h>

namespace Thor{

struct ThVecUnaryMinusTag
{

	template < class ReturnT, class ArgT, class IterT >
	static THOR_INLINE ReturnT apply( const ArgT& arg, const IterT& i )
	{
		return (-arg.at<ReturnT>(i));
	};

	

	

	/*template < class ReturnT, class ArgT >
	static THOR_INLINE ReturnT apply( const ArgT& arg )
	{
		return (-arg);
	};*/

	
};

struct ThVecBinaryPlusTag
{
	template < class ReturnT, class LhsT, class RhsT, class IterT >
	static THOR_INLINE ReturnT apply( const LhsT& lhs, const RhsT& rhs, const IterT& i )
	{
		return (lhs.at<ReturnT>(i) + rhs.at<ReturnT>(i));
	};	

	

	/*template < class ReturnT, class LhsT, class RhsT >
	static THOR_INLINE ReturnT apply( const LhsT& lhs, const RhsT& rhs )
	{
		return (lhs + rhs);
	};*/
};

struct ThMatMulVecTag
{
	template < class ReturnT, class LhsT, class RhsT, class IterT >
	static THOR_INLINE ReturnT apply( const LhsT& lhs, const RhsT& rhs, const IterT& i )
	{
		ReturnT res = ReturnT(0.0);
		for ( unsigned int j = 0; j < rhs.ref().Size(); ++j )
			res += lhs.at<ReturnT>(i,j) * rhs.at<ReturnT>(j);
		return res;
	};	

	

	/*template < class ReturnT, class LhsT, class RhsT >
	static THOR_INLINE ReturnT apply( const LhsT& lhs, const RhsT& rhs )
	{
		return (lhs + rhs);
	};*/
};

}//Thor

#endif