#ifndef THOR_FS_STLFILESTREAM_H
#define THOR_FS_STLFILESTREAM_H

#include <Thor/Core/Filesystem/FSLibConfig.h>
#include <Thor/Core/Filesystem/ThiFileStream.h>

namespace Thor{

//----------------------------------------------------------------------------------------
//
//					ThStlFileStream
//
//----------------------------------------------------------------------------------------
/*!
* \brief
* A stream for memory chunks handling.
*
*/
class THOR_FRAMEWORK_DLL ThStlFileStream:public ThiFileStream
{
public:

	ThStlFileStream();

	ThStlFileStream(const ThString& filename_,eStreamMode::Val streamMode_=eStreamMode::ReadMode,eFileWriteMode::Val writeMode_=eFileWriteMode::Append);

	~ThStlFileStream();	

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
	* STL file buffer.
	*/
	mutable std::filebuf	m_Buf;

	/*!
	* \brief
	* STL file input stream.
	*/
	mutable std::istream	m_In;

	/*!
	* \brief
	* STL file output stream.
	*/
	std::ostream			m_Out;

	/*!
	* \brief
	* STL file open mode.
	*/
	ThU32					m_Bitfield;
};

}//Thor

#endif