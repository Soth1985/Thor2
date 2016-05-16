#ifndef THOR_FS_THWINDYNAMICLIBRARY_H
#define THOR_FS_THWINDYNAMICLIBRARY_H

#include <Thor/Framework/Filesystem/ThiDynamicLibrary.h>
#include <windows.h>

namespace Thor
{
//----------------------------------------------------------------------------------------
//
//					ThWinDynamicLibrary
//
//----------------------------------------------------------------------------------------
class ThWinDynamicLibrary:public ThiDynamicLibrary
{
public:
	ThWinDynamicLibrary();

	ThWinDynamicLibrary(const ThString& name);

	ThWinDynamicLibrary(const ThString& name, HMODULE module);

	virtual ThBool				Load();

	virtual ThBool				IsLoaded();

	virtual void				Unload();	

	virtual const ThString&		GetName()const;

	virtual ThiType*			GetType()const;
	
protected:
	virtual void*				GetProcedureAddressImpl(const ThString& funcName);
private:

	HMODULE			m_Module;
	ThBool			m_Loaded;
	ThString		m_Name;
};

}

#endif