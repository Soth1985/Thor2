#include <Thor/Core/Filesystem/ThiDynamicLibrary.h>

namespace Thor
{

THOR_REG_TYPE(ThiDynamicLibrary, THOR_TYPELIST_1(ThiObject));
//----------------------------------------------------------------------------------------
//
//					ThiDynamicLibrary
//
//----------------------------------------------------------------------------------------
void* ThiDynamicLibrary::GetProcedureAddress(const ThString& funcName)
{
	if( !IsLoaded() )
	{
		Load();
	}

	if( IsLoaded() )
	{
		return GetProcedureAddressImpl(funcName);
	}
	else
		return 0;
}

}