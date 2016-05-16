#include <Thor/Framework/ThiObject.h>
#include <Thor/Framework/ThObjectTableInternal.h>

namespace Thor
{

THOR_REG_ROOT_TYPE(ThF32);
THOR_REG_ROOT_TYPE(ThF64);
THOR_REG_ROOT_TYPE(ThI8);
THOR_REG_ROOT_TYPE(ThU8);
THOR_REG_ROOT_TYPE(ThI16);
THOR_REG_ROOT_TYPE(ThU16);
THOR_REG_ROOT_TYPE(ThI32);
THOR_REG_ROOT_TYPE(ThU32);
THOR_REG_ROOT_TYPE(ThI64);
THOR_REG_ROOT_TYPE(ThU64);
THOR_REG_ROOT_TYPE(ThBool);
//THOR_REG_ROOT_TYPE(ThSize);
THOR_REG_ROOT_TYPE(ThString);
THOR_REG_ROOT_TYPE(ThWideString);

THOR_REG_TYPE(ThiObject, THOR_TYPELIST_1(ThiRtti));
//----------------------------------------------------------------------------------------
//
//					ThiObject
//
//----------------------------------------------------------------------------------------
ThiObject::ThiObject()
{

}
//----------------------------------------------------------------------------------------
ThString ThiObject::ToString() const
{
	ThString res;
	char buf[128] = "";
	sprintf(buf, "Object address is %p", this);
	res = buf;
	return res;
}
//----------------------------------------------------------------------------------------
ThiObject::~ThiObject()
{
	//
}
//----------------------------------------------------------------------------------------
ThiObjectPtr ThiObject::DeepCopy()
{
	return ThiObjectPtr(this);
}
//----------------------------------------------------------------------------------------
ThU32 ThiObject::GetHashCode()const
{
	return ThU32(this);
}
//----------------------------------------------------------------------------------------
ThiObjectPtr ThiObject::This()const
{
	return ThiObjectPtr(const_cast<ThiObject*>(this));
}

}//Thor
