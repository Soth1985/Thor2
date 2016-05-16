#include <Thor/Framework/Filesystem/PhysFileSystem.h>

namespace Thor
{
THOR_REG_TYPE(ThPhysFileSystem, THOR_TYPELIST_1(ThiFileSystem));
//----------------------------------------------------------------------------------------
//
//					ThPhysFileSystem
//
//----------------------------------------------------------------------------------------
ThPhysFileSystem::ThPhysFileSystem()
{
	int res=PHYSFS_init(0);

	if(res==0)
		throw FileSystemInitException( PHYSFS_getLastError() );

	MountDir(GetInitialPath(), "", eMountMode::FullAccess);
}
//----------------------------------------------------------------------------------------
ThPhysFileSystem::~ThPhysFileSystem()
{
	int res=PHYSFS_deinit();
	assert(res!=0);
}
//----------------------------------------------------------------------------------------
void ThPhysFileSystem::MountDir(const ThString& path, const ThString& target, eMountMode::Val mode)
{
	int res=0;
	
	if(mode==eMountMode::ReadOnly)
		res=PHYSFS_mount(path.c_str() , target.c_str(), 1);
	else if(mode==eMountMode::FullAccess)
	{
		res=PHYSFS_mount(path.c_str() , target.c_str(), 1);
		res&=PHYSFS_setWriteDir( path.c_str() );
	}

	if(res==0)
		throw MountFailedException(path + "  " + PHYSFS_getLastError() );
}
//----------------------------------------------------------------------------------------
void ThPhysFileSystem::DismountDir(const ThString& path)
{
	int res=PHYSFS_removeFromSearchPath( path.c_str() );
	assert(res);
	//const char *i=PHYSFS_getLastError();
	const char *p=PHYSFS_getWriteDir();
	if(p)
	{
		ThString tmp(p);
		if(p==path)
			PHYSFS_setWriteDir(0);
	}
}
//----------------------------------------------------------------------------------------
ThBool ThPhysFileSystem::Exists(const ThString& path)const
{
	return ( PHYSFS_exists( path.c_str() ) > 0 );
}
//----------------------------------------------------------------------------------------
ThiFileSystem::StringList ThPhysFileSystem::Enumerate(const ThString& path)
{
	StringList tmp;
	
	char **rc = PHYSFS_enumerateFiles( path.c_str() );			
	char **i;

	for (i = rc; *i != NULL; i++)
		tmp.push_back( ThString(*i) );

	PHYSFS_freeList(rc);

	return tmp;
}
//----------------------------------------------------------------------------------------
ThiFileSystem::StringList ThPhysFileSystem::GetSearchPaths()
{
	StringList tmp;

	char **rc= PHYSFS_getSearchPath() ;
	char **i;

	for (i = rc; *i != NULL; i++)
		tmp.push_back( ThString(*i) );

	PHYSFS_freeList(rc);

	return tmp;

}
//----------------------------------------------------------------------------------------
ThString ThPhysFileSystem::GetInitialPath()const
{
	return ThString( PHYSFS_getBaseDir() );
}
//----------------------------------------------------------------------------------------
void ThPhysFileSystem::MakeDirectory(const ThString& path)
{
	int res=PHYSFS_mkdir( path.c_str() );

	if(res==0)
		throw MakeDirException( path + ' ' + PHYSFS_getLastError() );
}
//----------------------------------------------------------------------------------------
ThiFileStreamPtr ThPhysFileSystem::OpenFile(const ThString& filename, eStreamMode::Val streamMode,eFileWriteMode::Val writeMode)
{
	if( Exists(filename) && streamMode==eStreamMode::ReadMode )
		return ThiFileStreamPtr( new ThPhysFileStream(filename, streamMode, writeMode) );
	else if(streamMode==eStreamMode::WriteMode || streamMode==eStreamMode::ReadWriteMode)
		return ThiFileStreamPtr( new ThPhysFileStream(filename, streamMode, writeMode) );
	else
	{
		return ThiFileStreamPtr(0);
		//throw OpenFileException( filename );
	}
}
//----------------------------------------------------------------------------------------
void ThPhysFileSystem::Delete(const ThString& path)
{
	int res=0;

	if( Exists(path) )
		res=PHYSFS_delete( path.c_str() );

	if(res==0)
		throw DeleteFileException( path + ' ' + PHYSFS_getLastError() );
}
//----------------------------------------------------------------------------------------
ThBool ThPhysFileSystem::IsDirectory(const ThString& path)
{
	return ( PHYSFS_isDirectory( path.c_str() ) > 0 );
}
//----------------------------------------------------------------------------------------
ThString ThPhysFileSystem::GetFullPath(const ThString& path)const
{
	if(Exists(path))
		return ThString( PHYSFS_getRealDir( path.c_str() ) ) + path;
	else 
		throw FileDoesNotExistException(path);
}
//----------------------------------------------------------------------------------------
ThiType* ThPhysFileSystem::GetType()const
{
	return Thor::TypeOf<ThPhysFileSystem>();
}

}//Thor
