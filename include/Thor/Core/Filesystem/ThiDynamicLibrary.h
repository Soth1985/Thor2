#ifndef THOR_FS_THIDYNAMICLIBRARY_H
#define THOR_FS_THIDYNAMICLIBRARY_H

#include <Thor/Core/Filesystem/FSLibConfig.h>

namespace Thor
{
//----------------------------------------------------------------------------------------
//
//					ThiDynamicLibrary
//
//----------------------------------------------------------------------------------------
class ThiDynamicLibrary:public ThiObject
{
public:
	virtual ThBool				Load()=0;
	virtual ThBool				IsLoaded()=0;
	virtual void				Unload()=0;	
	virtual const ThString&		GetName()const=0;
	void*						GetProcedureAddress(const ThString& funcName);
protected:
	virtual void*				GetProcedureAddressImpl(const ThString& funcName)=0;
};

}

#endif