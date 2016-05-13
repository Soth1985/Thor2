#include <Thor/Framework/Filesystem/ThiDataStream.h>

namespace Thor{	

THOR_REG_TYPE(ThiDataStream, THOR_TYPELIST_1(ThiObject));
//----------------------------------------------------------------------------------------
//
//					ThiDataStream
//
//----------------------------------------------------------------------------------------
eStreamMode::Val ThiDataStream::GetStreamMode() const
{
	return m_Mode;
};
//----------------------------------------------------------------------------------------
ThSize ThiDataStream::Read(void* buf, ThSize bytes)const
{
	Open();

	mutex_t::scoped_lock lock(m_Mutex);

	if( (m_Mode==eStreamMode::ReadMode) || (m_Mode==eStreamMode::ReadWriteMode) )
		return ReadImpl(buf,bytes);
	else
		return 0;
}
//----------------------------------------------------------------------------------------
ThSize ThiDataStream::Write(const void* buf, ThSize bytes)
{
	Open();

	mutex_t::scoped_lock lock(m_Mutex);

	if( (m_Mode==eStreamMode::WriteMode) || (m_Mode==eStreamMode::ReadWriteMode) )
		return WriteImpl(buf,bytes);
	else 
		return 0;
}
//----------------------------------------------------------------------------------------
ThSize ThiDataStream::Tell()const
{
	Open();

	mutex_t::scoped_lock lock(m_Mutex);

	return TellImpl();
}
//----------------------------------------------------------------------------------------
void ThiDataStream::Seek(ThSize pos)const
{
	Open();

	mutex_t::scoped_lock lock(m_Mutex);

	SeekImpl(pos);
}		
//----------------------------------------------------------------------------------------
ThSize ThiDataStream::GetSize()const
{
	Open();

	mutex_t::scoped_lock lock(m_Mutex);

	return GetSizeImpl();
}
//----------------------------------------------------------------------------------------
ThBool ThiDataStream::Eof()const
{
	Open();

	mutex_t::scoped_lock lock(m_Mutex);

	return EofImpl();
}
//----------------------------------------------------------------------------------------
ThiDataStream::ThiDataStream(eStreamMode::Val mode_, ThBool isOpened_)
	:
m_Mode(mode_),
m_IsOpened(isOpened_)
{
	//
};		
//----------------------------------------------------------------------------------------
ThiDataStream::~ThiDataStream()
{
	Close();
};
//----------------------------------------------------------------------------------------
void ThiDataStream::Open()const
{
	if( !m_IsOpened )
	{
		mutex_t::scoped_lock lock(m_Mutex);
		if(!m_IsOpened)
		{
			OpenImpl();
			m_IsOpened=true;
		}
	}
}
//----------------------------------------------------------------------------------------
void ThiDataStream::Close()const
{
	mutex_t::scoped_lock lock(m_Mutex);

	if(m_IsOpened)
	{
		CloseImpl();
		m_IsOpened=false;
	}
};
//----------------------------------------------------------------------------------------
void ThiDataStream::OpenImpl()const
{
	//
};
//----------------------------------------------------------------------------------------
void ThiDataStream::CloseImpl()const
{
	//
};

}//Thor