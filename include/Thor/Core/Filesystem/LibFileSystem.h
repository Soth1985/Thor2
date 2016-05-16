#ifndef THOR_LIB_FILESYSTEM_H
#define THOR_LIB_FILESYSTEM_H

#include <Thor/Core/Filesystem/ThiFileSystem.h>
#include <Thor/Core/Filesystem/ThiDataStream.h>
#include <Thor/Core/Filesystem/ThiFileStream.h>
#include <Thor/Core/Filesystem/ThiMemoryStream.h>
#include <Thor/Core/Filesystem/FSExceptions.h>
#include <Thor/Core/Filesystem/LoggerFileOutput.h>
#include <Thor/Core/Containers/Singleton.h>

namespace Thor
{
//----------------------------------------------------------------------------------------
//
//					eFileSystemType
//
//----------------------------------------------------------------------------------------
struct eFileSystemType
{
	enum Val
	{
		PhysFS,
		BoostFS
	};
};
//----------------------------------------------------------------------------------------
//
//					ThFileSystemFactory
//
//----------------------------------------------------------------------------------------
class THOR_FRAMEWORK_DLL ThFileSystemFactory:public NonCopyable
{
public:
	ThiFileSystemPtr CreateFileSystem(eFileSystemType::Val type);

	static ThFileSystemFactory& Instance();
protected:

	friend class Singleton<ThFileSystemFactory>;
	typedef Singleton<ThFileSystemFactory> singleton_t;

	ThFileSystemFactory();	
};

}//Thor
#endif