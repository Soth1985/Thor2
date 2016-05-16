#ifndef THOR_FS_PHYSFILESTREAM_H
#define THOR_FS_PHYSFILESTREAM_H

#include <Thor/Core/Filesystem/FSLibConfig.h>
#include <Thor/Core/Filesystem/ThiFileStream.h>
#include <PhysFS/physfs.h>

namespace Thor
{
//----------------------------------------------------------------------------------------
//
//					ThPhysFileStream
//
//----------------------------------------------------------------------------------------
class THOR_FRAMEWORK_DLL ThPhysFileStream:public ThiFileStream
{
public:

	ThPhysFileStream(const ThString& filename_,eStreamMode::Val streamMode_=eStreamMode::ReadMode,eFileWriteMode::Val writeMode_=eFileWriteMode::Append);

	ThPhysFileStream();

	~ThPhysFileStream();	

	virtual void Flush();

protected:

	/*!
	* \copydoc DataStream::Open
	*/
	virtual void OpenImpl()const;

	/*!
	* \copydoc DataStream::Close
	*/
	virtual void CloseImpl()const;

	/*!
	* \copydoc DataStream::Tell
	*/
	virtual ThSize TellImpl()const;

	/*!
	* \copydoc DataStream::Seek
	*/
	virtual void SeekImpl(ThSize pos)const;

	/*!
	* \copydoc DataStream::GetSize
	*/
	virtual ThSize GetSizeImpl()const;

	/*!
	* \copydoc DataStream::Eof
	*/
	virtual ThBool EofImpl()const;

	/*!
	* \copydoc DataStream::ReadImpl
	*/
	virtual ThSize ReadImpl(void* buf, ThSize bytes)const;

	/*!
	* \copydoc DataStream::WriteImpl
	*/
	virtual ThSize WriteImpl(const void* buf, ThSize bytes);

	virtual ThiType*	GetType()const;

private:
	/*!
	* \brief
	* PhysFS internal handle.
	*/
	mutable PHYSFS_File*	m_Fp;

	/*!
	* \brief
	* Path to a file in a virtual filesystem.
	*/
	ThString				m_VirtualName;
};

}//Thor

#endif