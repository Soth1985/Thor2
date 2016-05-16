#ifndef THOR_FS_THIMEMORYSTREAM_H
#define THOR_FS_THIMEMORYSTREAM_H

#include <Thor/Framework/Filesystem/FSLibConfig.h>
#include <Thor/Framework/Filesystem/ThiDataStream.h>
#include <string.h>

namespace Thor
{
//----------------------------------------------------------------------------------------
//
//					ThMemoryStream
//
//----------------------------------------------------------------------------------------
/*!
 * \brief
 * A stream for memory chunks handling.
 *
 */
class THOR_FRAMEWORK_DLL ThiMemoryStream:public ThiDataStream
{
public:

	/*!
	 * \brief
	 * Creates a memory stream from an existing chunk of memory. Provided memory buffer is not freed by the stream. 
	 * 
	 * \param beg_
	 * Pointer to the beginning of the memory chunk.
	 * 
	 * \param size
	 * Size of the memory chunk.
	 * 
	 * \param mode
	 * Read access mode of the stream.
	 * 
	 */
	ThiMemoryStream(ThU8* beg_, ThSize size, eStreamMode::Val mode=eStreamMode::ReadMode);

	/*!
	* \brief
	* Creates a memory stream from an allocated buffer.
	* 
	* 
	* \param size
	* Size of the buffer.
	* 
	* \param mode
	* Read access mode of the stream.
	* 
	*/
	ThiMemoryStream(ThSize size, eStreamMode::Val mode=eStreamMode::ReadMode);

	ThiMemoryStream(eStreamMode::Val mode=eStreamMode::ReadMode);
	
	/*!
	* \brief
	* Increases the size of memory buffer. 
	* 
	* \param addSize
	* Additional size to grow by.
	* 		
	* 
	*/
	void				Grow(ThSize addSize);

	virtual void		Flush();	

	~ThiMemoryStream();		

protected:

	/*!
	* \copydoc DataStream::Tell
	*/
	virtual ThSize		TellImpl();

	/*!
	* \copydoc DataStream::Seek
	*/
	virtual void		SeekImpl(ThSize pos);

	/*!
	* \copydoc DataStream::GetSize
	*/
	virtual ThSize		GetSizeImpl();

	/*!
	* \copydoc DataStream::Eof
	*/
	virtual ThBool		EofImpl();

	/*!
	* \copydoc DataStream::ReadImpl
	*/
	virtual ThSize		ReadImpl(void* buf, ThSize bytes)const;

	/*!
	* \copydoc DataStream::WriteImpl
	*/
	virtual ThSize		WriteImpl(const void* buf, ThSize bytes);

private:

	
	/*!
	* \brief
	* Beginning of the memory chunk.
	*/
	ThU8* m_Beg;

	/*!
	* \brief
	* Current offset into the stream.
	*/
	mutable ThU8* m_Cur;

	/*!
	* \brief
	* End position of the stream.
	*/
	ThU8* m_End;
	
	/*!
	* \brief
	* Determines if the memory should be deallocated upon stream destruction.
	*/
	bool m_FreeMem;
};	

}//Thor

#endif