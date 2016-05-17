#ifndef THOR_FS_THIDATASTREAM_H
#define THOR_FS_THIDATASTREAM_H

#include <Thor/Core/Filesystem/FSLibConfig.h>
#include <Thor/Core/SmartPointers/RefPtr.h>

namespace Thor{
//----------------------------------------------------------------------------------------
//
//					eFileWriteMode
//
//----------------------------------------------------------------------------------------
struct eFileWriteMode
{
	enum Val
	{
		Append,
		Truncate
	};
};
//----------------------------------------------------------------------------------------
//
//					eStreamMode
//
//----------------------------------------------------------------------------------------
struct eStreamMode
{
	/*!
	* \brief
	* A stream can be opened in one of the following modes.
	*	-Read
	*	-Write
	*	-ReadWrite
	* 
	*/
	enum Val
	{
		ReadMode,
		WriteMode,
		ReadWriteMode
	};
};
//----------------------------------------------------------------------------------------
//
//					ThiDataStream
//
//----------------------------------------------------------------------------------------
/*!
 * \brief
 * Base class for file and memory streams.
 * 
 * Defines a base interface for steam operations.
 * You can delay the opening of the stream until it is first operated on by passing isOpened_=false
 * into the DataStream constructor from the derived classes.
 * 
 */
class THOR_FRAMEWORK_DLL ThiDataStream:public ThiObject
{
public:		

	/*!
	 * \brief
	 * Returns the mode this stream was opened with.
	 * 
	 */
	eStreamMode::Val GetStreamMode() const;

	/*!
	* \brief
	* Reads 'bytes' amount of bytes into the buffer buf.
	* \param buf
	* Buffer to write to.
	* \param bytes
	* Number of bytes that shall be read from the stream.
	*
	* The implementation is also expected to move current stream position
	* to point after the read chunk.Returns the number of bytes read.
	* 
	*/
	ThSize Read(void* buf, ThSize bytes)const;		

	/*!
	* \brief
	* Writes 'bytes' amount of bytes into the stream from the buffer buf.
	* \param buf
	* Buffer to read data from.
	* \param bytes
	* Number of bytes that shall be written from the buffer.
	*
	* The implementation is also expected to move current stream position
	* to point after the written chunk.Returns the number of bytes written.
	* 
	*/
	ThSize Write(const void* buf, ThSize bytes);

	/*!
	* \brief
	* Returns current offset from the beginning of the stream.
	* 
	*/
	ThSize Tell()const;

	/*!
	* \brief
	* Moves current position pos bytes away from the stream start.
	*
	* \param pos 
	* Number of bytes to move.
	* 
	*/
	void Seek(ThSize pos)const;
	
	/*!
	* \brief
	* If possible returns the size(in bytes) of stream buffer.
	*
	*/
	ThSize GetSize()const;

	/*!
	* \brief
	* Returns true if current stream position has reached the end of stream.
	*
	*/
	ThBool Eof()const;		

	/*!
	* \brief
	* Creates a stream with a given access mode.
	* \param mode_
	* Access mode of the stream.
	*
	*/
	ThiDataStream(eStreamMode::Val mode_, ThBool isOpened_);	

	/*!
	* \brief
	* Forces all operations with the stream that may be buffered up to complete.
	*
	*/
	virtual void Flush()=0;

	/*!
	* \brief
	* Opens the stream. Does nothing by default.
	* 
	*/
	void Open()const;

	/*!
	* \brief
	* Closes the stream. Does nothing by default.
	* 
	*/
	void Close()const;

	virtual ~ThiDataStream();

protected:		

	virtual void OpenImpl()const;
	virtual void CloseImpl()const;		
	virtual ThSize ReadImpl(void* buf, ThSize bytes)const=0;
	virtual ThSize WriteImpl(const void* buf, ThSize bytes)=0;
	virtual ThSize TellImpl()const=0;
	virtual void SeekImpl(ThSize pos)const=0;
	virtual ThSize GetSizeImpl()const=0;
	virtual ThBool EofImpl()const=0;

	/*!
	 * \brief
	 * Stream access mode.
	 */
	eStreamMode::Val m_Mode;

	/*!
	* \brief
	* Determines if the stream is opened, used for lazy initialization of the stream.
	*/
	mutable ThBool m_IsOpened;
};

}//Thor

#endif