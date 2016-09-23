#ifndef THOR_FS_PHYSFILESYSTEM_H
#define THOR_FS_PHYSFILESYSTEM_H

#include <Thor/Core/Filesystem/FSLibConfig.h>
#include <Thor/Core/Filesystem/PhysFileStream.h>
#include <Thor/Core/Filesystem/ThiFileSystem.h>
#include <Thor/Core/Filesystem/FSExceptions.h>

namespace Thor
{
//----------------------------------------------------------------------------------------
//
//					ThPhysFileSystem
//
//----------------------------------------------------------------------------------------
class THOR_FRAMEWORK_DLL ThPhysFileSystem:public ThiFileSystem
{
public:

	ThPhysFileSystem();

	~ThPhysFileSystem();

	/*!
	* \copydoc FileSystem::MountDir
	*/
	virtual void MountDir(const ThString& path, const ThString& target=ThString(), eMountMode::Val mode=eMountMode::ReadOnly);

	/*!
	* \copydoc FileSystem::DismountDir
	* May fail if the dismounted directory contains opened files.
	*/
	virtual void DismountDir(const ThString& path);

	/*!
	* \copydoc FileSystem::Exists
	*/
	virtual bool Exists(const ThString& path)const;

	/*!
	* \copydoc FileSystem::Enumerate
	*/
	virtual StringList Enumerate(const ThString& path);

	/*!
	* \copydoc FileSystem::GetSearchPaths
	*/
	virtual StringList GetSearchPaths();

	/*!
	* \copydoc FileSystem::MakeDirectory
	*/
	virtual void MakeDirectory(const ThString& path);

	/*!
	* \copydoc FileSystem::OpenFile
	*/
	virtual ThiFileStreamPtr OpenFile(const ThString& filename, eStreamMode::Val streamMode=eStreamMode::ReadMode,eFileWriteMode::Val writeMode=eFileWriteMode::Append);

	/*!
	* \copydoc FileSystem::Delete
	*/
	virtual void Delete(const ThString& path);

	/*!
	* \copydoc FileSystem::IsDirectory
	*/
	virtual bool IsDirectory(const ThString& path);

	/*!
	* \copydoc FileSystem::GetFullPath
	*/
	virtual ThString GetFullPath(const ThString& path)const;

	/*!
	* \copydoc FileSystem::GetInitialPath
	*/
	virtual ThString GetInitialPath()const;

	virtual ThiType*	GetType()const;
};

}//Thor

#endif