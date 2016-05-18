#include <Thor/Core/Filesystem/ThWinDynamicLibrary.h>

namespace Thor
{
THOR_REG_TYPE(ThWinDynamicLibrary, THOR_TYPELIST_1(ThiDynamicLibrary));
//----------------------------------------------------------------------------------------
//
//					ThWinDynamicLibrary
//
//----------------------------------------------------------------------------------------
ThWinDynamicLibrary::ThWinDynamicLibrary()
	:
m_Loaded(false)
{
	//
}
//----------------------------------------------------------------------------------------
ThWinDynamicLibrary::ThWinDynamicLibrary(const ThString& name, HMODULE module)
	:
m_Loaded(true),
m_Name(name),
m_Module(module)
{
	//
}
//----------------------------------------------------------------------------------------
ThWinDynamicLibrary::ThWinDynamicLibrary(const ThString& name)
	:
m_Loaded(false),
m_Name(name)
{
	//
}
//----------------------------------------------------------------------------------------
ThBool ThWinDynamicLibrary::Load()
{
	mutex_t::scoped_lock lock(m_Mutex);

	if(!m_Loaded)
	{			
		m_Module = LoadLibrary(TEXT( m_Name.c_str() ));
		if (m_Module == NULL) 
		{
			THOR_ERR("Failed to load a dynamic library %s")(frameworkSysLogTag, m_Name.c_str() );
			return false;
		}
		else
		{
			m_Loaded = true;
			return true;
		}
	}
	else
		return true;
}
//----------------------------------------------------------------------------------------
ThBool ThWinDynamicLibrary::IsLoaded()
{
	return m_Loaded;
}
//----------------------------------------------------------------------------------------
void ThWinDynamicLibrary::Unload()
{
	mutex_t::scoped_lock lock(m_Mutex);

	if( IsLoaded() )
	{
		FreeLibrary(m_Module);
	}
}	
//----------------------------------------------------------------------------------------
const ThString& ThWinDynamicLibrary::GetName()const
{
	return m_Name;
}
//----------------------------------------------------------------------------------------
void* ThWinDynamicLibrary::GetProcedureAddressImpl(const ThString& funcName)
{		
	return GetProcAddress(m_Module, funcName.c_str());
}
//----------------------------------------------------------------------------------------
ThiType* ThWinDynamicLibrary::GetType()const
{
	return Thor::TypeOf<ThWinDynamicLibrary>();
}

}