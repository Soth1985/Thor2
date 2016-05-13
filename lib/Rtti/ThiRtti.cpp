#include <Thor/Framework/ThiRtti.h>

namespace Thor
{
THOR_REG_ROOT_TYPE(ThiRtti);
//----------------------------------------------------------------------------------------
//
//					ThiRtti
//
//----------------------------------------------------------------------------------------
ThiRtti::~ThiRtti()
{
	//
}
//----------------------------------------------------------------------------------------
ThUUID ThiRtti::GetUUID()const
{
	static ThUUID sUUID;
	return sUUID;
}

}