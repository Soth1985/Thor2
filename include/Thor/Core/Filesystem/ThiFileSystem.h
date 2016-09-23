#ifndef THOR_FS_FILESYSTEM_H
#define THOR_FS_FILESYSTEM_H

#include <Thor/Core/Filesystem/FSLibConfig.h>
#include <Thor/Core/Filesystem/ThiFileStream.h>
#include <Thor/Core/Filesystem/ThiDynamicLibrary.h>
#include <Thor/Core/SmartPointers/RefPtr.h>
#include <vector>
#include <string>

namespace Thor
{	
//----------------------------------------------------------------------------------------
//
//					eMountMode
//
//----------------------------------------------------------------------------------------
struct eMountMode
{
	/*!
	* \brief
	* Directory mount mode.
	* 
	* It can be one of the following:
	* -ReadOnly can only read files in these dirs.
	* -FullAccess can write to files in this dir and any of it`s children
	* 
	*/
	enum Val
	{
		ReadOnly,
		FullAccess
	};
};
//----------------------------------------------------------------------------------------
//
//					ThiFileSystem
//
//----------------------------------------------------------------------------------------
/*!
 * \brief
 * Abstracts disk operations.
 * 
 * FileSystem should be used in the following way:
 * -Mount a number of paths to search in (note that at any time there 
 * can be only one directory with FullAccess mode, so if you wish to write 
 * to a directory that is not located in the current write dir tree, you 
 * should mount it as a new write dir). Directory from which the program is run
 * is set as a write dir by default.
 * -Access files and directories like this: "/mydir1/mydir2/myfile.txt"
 * 
 *
 */
class THOR_FRAMEWORK_DLL ThiFileSystem: public ThiObject
{
public:

	typedef std::vector< ThString > StringList;		
	
	/*!
	 * \brief
	 * Mount a directory into a search path.
	 * 
	 * \param path
	 * Os specific path to a directory.
	 * 
	 * \param target
	 * Point in the virtual file system where this dir will be attached to (currently makes sense only for PhysFS implementation).
	 * 
	 * \param mode
	 * Access mode of this directory.
	 * 
	 * \throws MountFailedException
	 * Throws if the path dir does not exists.
	 * 
	 * There can be only one dir with FullAccess.
	 * 
	 */
	virtual void MountDir(const ThString& path, const ThString& target=ThString(), eMountMode::Val mode=eMountMode::ReadOnly)=0;
	
	/*!
	 * \brief
	 * Removes a directory from search path.
	 * 
	 * \param path
	 * Directory to remove.
	 * 
	 */
	virtual void DismountDir(const ThString& path)=0;
	
	/*!
	 * \brief
	 * Returns true if the requested file or directory exists.
	 * 
	 * \param path
	 * Path to check for existance.
	 * 
	 */
	virtual bool Exists(const ThString& path)const=0;

	/*!
	 * \brief
	 * Enumerates contents of a given directory.
	 * 
	 * \param path
	 * Directory to check.
	 * 
	 */
	virtual StringList Enumerate(const ThString& path)=0;

	/*!
	 * \brief
	 * Returns a list of file system`s search paths.
	 * 
	 */
	virtual StringList GetSearchPaths()=0;

	/*!
	 * \brief
	 * Makes a new directory.
	 * 
	 * \param path
	 * Path to a new directory.
	 * 
	 * \throws MakeDirException
	 * Throws if failed to create a directory.
	 * 
	 *
	 */
	virtual void MakeDirectory(const ThString& path)=0;

	/*!
	 * \brief
	 * Opens a file.
	 * 
	 * \param filename
	 * Path to a required file.
	 * 
	 * \param streamMode
	 * Mode of the file stream see DataStream.
	 * 
	 * \param writeMode
	 * Write mode of the file see FileStream.
	 * 
	 * \returns
	 * A file stream.
	 * 
	 * \throws OpenFileException
	 * Throws if the file could not be opened.
	 * 
	 */
	virtual ThiFileStreamPtr OpenFile(const ThString& filename, eStreamMode::Val streamMode=eStreamMode::ReadMode,eFileWriteMode::Val writeMode=eFileWriteMode::Append)=0;
	
	/*!
	 * \brief
	 * Deletes a file or directory.
	 * 
	 * \param path
	 * Path to a required file/dir.
	 * 
	 * \throws DeleteFileException
	 * Throws if failed to delete.
	 * 
	 */
	virtual void Delete(const ThString& path)=0;

	/*!
	 * \brief
	 * Returns true if the path represents a directory.
	 * 
	 * \param path
	 * Tested path.
	 * 
	 */
	virtual bool IsDirectory(const ThString& path)=0;

	/*!
	 * \brief
	 * Returns full path to a file.
	 * 
	 */
	virtual ThString GetFullPath(const ThString& path)const=0;

	/*!
	* \brief
	* Returns the path from where the program is run.
	* 
	*/
	virtual ThString GetInitialPath()const=0;

	/*!
	* \brief
	* Attempts to load a dynamic library.
	* 
	*/
	ThiDynamicLibraryPtr LoadDynamicLibrary(const ThString& path);

	ThString GetFileExtension(const ThString& path);

private:

	typedef std::unordered_map<ThString, ThiDynamicLibraryPtr>  DllList;

	DllList m_DllList;
};

}//Thor

#endif