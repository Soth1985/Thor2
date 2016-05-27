#include <Thor/Core/Filesystem/ThiFileSystem.h>

#ifdef THOR_MS_WIN
	#include <Thor/Framework/Filesystem/ThWinDynamicLibrary.h>
#endif

namespace Thor
{

THOR_REG_TYPE(ThiFileSystem, THOR_TYPELIST_1(ThiObject));
//----------------------------------------------------------------------------------------
//
//					ThiFileSystem
//
//----------------------------------------------------------------------------------------
ThiDynamicLibraryPtr ThiFileSystem::LoadDynamicLibrary(const ThString& path)
{
	//check if the library was already requested
	DllList::iterator i = m_DllList.find(path);

	if( i != m_DllList.end() )
		return i->second;

	//load the library
	if( Exists(path) )
	{
		ThiDynamicLibraryPtr result;
#ifdef THOR_MS_WIN
		//check if the library was previously loaded bypassing the framework api
		HMODULE module = GetModuleHandle( path.c_str() );

		if(module == NULL)
			result = ThiDynamicLibraryPtr( new ThWinDynamicLibrary(path) );
		else
			result = ThiDynamicLibraryPtr( new ThWinDynamicLibrary(path, module) );

		m_DllList.insert( DllList::value_type(path, result) );
#else
		THOR_ERR("Dynamic libraries are not supported on this platform")(frameworkSysLogTag);
#endif
		return result;
	}
	else
	{
		return ThiDynamicLibraryPtr();
	}
}

ThString ThiFileSystem::GetFileExtension(const ThString& path)
{
	ThString result;

	ThSize pos = path.find_last_of('.');

	if (pos != ThString::npos)
	{
		result = path.substr(pos);
	}

	return result;
}

}//Thor