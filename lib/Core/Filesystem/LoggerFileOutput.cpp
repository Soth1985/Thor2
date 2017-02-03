#include <Thor/Core/Filesystem/LoggerFileOutput.h>
#include <Thor/Core/String/String.h>

namespace Thor
{

THOR_REG_TYPE(ThLoggerFileOutput, THOR_TYPELIST_1(ThiLoggerOutputTarget));
//----------------------------------------------------------------------------------------
//
//					ThLoggerFileOutput
//
//----------------------------------------------------------------------------------------
ThLoggerFileOutput::ThLoggerFileOutput()
{
	//
}
//----------------------------------------------------------------------------------------
ThLoggerFileOutput::ThLoggerFileOutput(ThiFileStreamPtr& file)
	:
m_File(file)
{
	//
}
//----------------------------------------------------------------------------------------
void ThLoggerFileOutput::Print(ThChar* str)
{
	if(m_File)
	{
		m_File->Write(str, strlen(str) * sizeof(ThChar) );
		m_File->Flush();
	}
}
//----------------------------------------------------------------------------------------
void ThLoggerFileOutput::Print(ThWchar* str)
{
	if(m_File)
	{
		m_File->Write(str, wcslen(str) * sizeof(ThWchar) );
		m_File->Flush();
	}
}
//----------------------------------------------------------------------------------------
ThiType* ThLoggerFileOutput::GetType()const
{
	return ThType<ThLoggerFileOutput>::Instance();
}
//----------------------------------------------------------------------------------------
void ThLoggerFileOutput::SetFile(ThiFileStreamPtr& file)
{
	m_File=file;
}


}//Thor
