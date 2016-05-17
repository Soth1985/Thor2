#include <Thor/Core/Filesystem/StdFileSystem.h>

namespace Thor
{
THOR_REG_TYPE(ThBoostFileSystem, THOR_TYPELIST_1(ThiFileSystem));
//----------------------------------------------------------------------------------------
//
//					ThBoostFileSystem
//
//----------------------------------------------------------------------------------------
ThBoostFileSystem::ThBoostFileSystem()
{
	MountDir(GetInitialPath(), "", eMountMode::FullAccess);
}
//----------------------------------------------------------------------------------------
void ThBoostFileSystem::MountDir(const ThString& path, const ThString& target, eMountMode::Val mode)
{
	mutex_t::scoped_lock lock(m_Mutex);

	if( fs::exists( fs::path( path ) ) )
	{
		m_PathList.push_back( Path(path) );
		if(mode==eMountMode::FullAccess)
			m_WriteDir=fs::path(path);
	}
	else
		throw MountFailedException(path + " does not exist" );
};
//----------------------------------------------------------------------------------------
void ThBoostFileSystem::DismountDir(const ThString& path)
{
	mutex_t::scoped_lock lock(m_Mutex);

	path_list_t::iterator i;

	for(i=m_PathList.begin(); i!=m_PathList.end(); ++i)
	{
		if(i->path==path)
		{
			break;
		}
	}

	if( i!=m_PathList.end() )
		m_PathList.erase(i);
}
//----------------------------------------------------------------------------------------
ThBool ThBoostFileSystem::Exists(const ThString& path)const
{			
	return ExistsIn(path, ThString() );
}
//----------------------------------------------------------------------------------------
ThiFileSystem::string_list ThBoostFileSystem::Enumerate(const ThString& path)
{
	path_list_t::iterator i;
	ThString str;
	//test 
	if( ExistsIn(path,str) )
		return EnumerateDir(str);
	else
		return string_list();
}
//----------------------------------------------------------------------------------------
ThString ThBoostFileSystem::GetInitialPath()const
{
	return fs::initial_path().string();
}
//----------------------------------------------------------------------------------------
ThiFileSystem::string_list ThBoostFileSystem::GetSearchPaths()
{
	mutex_t::scoped_lock lock(m_Mutex);

	string_list tmp;
	path_list_t::iterator i;

	for(i=m_PathList.begin(); i!=m_PathList.end(); ++i)
		tmp.push_back( i->path.string() );

	return tmp;
}
//----------------------------------------------------------------------------------------
void ThBoostFileSystem::MakeDirectory(const ThString& path)
{
	mutex_t::scoped_lock lock(m_Mutex);

	fs::create_directory(m_WriteDir / fs::path(path));
}
//----------------------------------------------------------------------------------------
ThiFileStreamPtr ThBoostFileSystem::OpenFile(const ThString& filename, eStreamMode::Val streamMode,eFileWriteMode::Val writeMode )
{
	ThString str;

	if( ExistsIn(filename, str) && streamMode==eStreamMode::ReadMode )
		return ThiFileStreamPtr( new ThStlFileStream(str, streamMode, writeMode) );
	else if(streamMode==eStreamMode::WriteMode || streamMode==eStreamMode::ReadWriteMode)
		return ThiFileStreamPtr( new ThStlFileStream(filename,streamMode,writeMode) );
	else
		throw OpenFileException( str );
}	
//----------------------------------------------------------------------------------------
void ThBoostFileSystem::Delete(const ThString& path)
{
	mutex_t::scoped_lock lock(m_Mutex);

	ThString str;
	if( ExistsIn( path,str ) )
			fs::remove( fs::path(str) );
}
//----------------------------------------------------------------------------------------
ThBool ThBoostFileSystem::IsDirectory(const ThString& path)
{
	path_list_t::iterator i;
	//try search paths first
	ThString str;

	if( ExistsIn(path,str) )
		fs::is_directory( fs::path(str) );
	
	//then try filesystem
	return fs::is_directory( fs::path(path) );
}
//----------------------------------------------------------------------------------------
ThString ThBoostFileSystem::GetFullPath(const ThString& path)const
{
	ThString str;

	if(ExistsIn(path,str))
		return str;
	else 
		throw FileDoesNotExistException(path);
}
//----------------------------------------------------------------------------------------
ThiFileSystem::string_list ThBoostFileSystem::EnumerateDir(const ThString& path)
{
	string_list tmp;
	fs::directory_iterator end_iter;
	fs::path full_path(path);
	for ( fs::directory_iterator dir_itr( full_path );
		dir_itr != end_iter;
		++dir_itr )
	{				
		tmp.push_back( dir_itr->path().string() );
	}

	return tmp;
}		
//----------------------------------------------------------------------------------------
ThBool ThBoostFileSystem::ExistsIn(const ThString& path, ThString& res)const
{
	mutex_t::scoped_lock lock(m_Mutex);

	fs::path seek(path);
	//look in search paths first
	path_list_t::const_iterator i;

	for(i=m_PathList.begin(); i!=m_PathList.end() ; ++i)
	{
		if( fs::exists(i->path / seek) )
		{
			res= (i->path / seek).string();
			return true;
		}
	}
	//look in write dir
	if( fs::exists(m_WriteDir / seek) )
	{
		res= (m_WriteDir / seek).string();
		return true;
	}
	//look in the filesystem, works against PhysFS
	/*if( fs::exists(seek) )
	{
		res=path;
		return true;
	}	*/
	
	return false;
}	
//----------------------------------------------------------------------------------------
ThiType* ThBoostFileSystem::GetType()const
{
	return Thor::GetType<ThBoostFileSystem>();
}

}//Thor
