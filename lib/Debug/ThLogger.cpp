#include <Thor/Framework/ThLogger.h>
#include <Thor/Framework/String.h>
#include <Thor/Framework/ThStringUtilities.h>
#include <windows.h>

namespace Thor{

THOR_REG_TYPE(ThiLoggerOutputTarget, THOR_TYPELIST_1(ThiObject));
THOR_REG_TYPE(ThLoggerDebuggerOutput, THOR_TYPELIST_1(ThiLoggerOutputTarget));
THOR_REG_TYPE(ThLoggerConsoleOutput, THOR_TYPELIST_1(ThiLoggerOutputTarget));
//----------------------------------------------------------------------------------------
//
//					ThLoggerDebuggerOutput
//
//----------------------------------------------------------------------------------------
void ThLoggerDebuggerOutput::Print(ThI8* str)
{
	//output into the debugger console	
#ifdef THOR_MS_WIN
	OutputDebugStringA(str);
#endif	
}

void ThLoggerDebuggerOutput::Print(ThWchar* str)
{
#ifdef THOR_MS_WIN
	OutputDebugStringW(str);
#endif
}
//----------------------------------------------------------------------------------------
ThiType* ThLoggerDebuggerOutput::GetType()const
{
	return ThType<ThLoggerDebuggerOutput>::Instance();
}
//----------------------------------------------------------------------------------------
//
//					ThLoggerConsoleOutput
//
//----------------------------------------------------------------------------------------
void ThLoggerConsoleOutput::Print(ThI8* str)
{
	//output to the console
	printf("%s", str);
}
//----------------------------------------------------------------------------------------
void ThLoggerConsoleOutput::Print(ThWchar* str)
{
	wprintf(L"%s", str);
}
//----------------------------------------------------------------------------------------
ThiType* ThLoggerConsoleOutput::GetType()const
{
	return ThType<ThLoggerConsoleOutput>::Instance();
}
//----------------------------------------------------------------------------------------
//
//					ThFormatHelper
//
//----------------------------------------------------------------------------------------
ThFormatHelper::ThFormatHelper()
{
	m_FormatSize = 128;
	m_Format = new ThI8[m_FormatSize];

	m_WideFormatSize = 128;
	m_WideFormat = new ThWchar[m_WideFormatSize];
}
//----------------------------------------------------------------------------------------
ThFormatHelper::~ThFormatHelper()
{
	delete[] m_Format;
	delete[] m_WideFormat;
}
//----------------------------------------------------------------------------------------
void ThFormatHelper::operator()(const ThI8* mid, ...)
{
	if(strlen(m_Format) > 0)
	{
		va_list args;
		va_start(args, mid);
		
		ThLogger::Instance().Log(mid, m_Format, args);

		va_end(args);
	}

	if (wcslen(m_WideFormat) > 0)
	{
		va_list args;
		va_start(args, mid);

		ThLogger::Instance().Log(mid, m_WideFormat, args);

		va_end(args);
	}
}
//----------------------------------------------------------------------------------------
//
//					ThLogger
//
//----------------------------------------------------------------------------------------
ThLogger::ThLogger()
{			
	m_BufferSize = 256;
	m_Buffer = new ThI8[m_BufferSize];
	m_WideBufferSize = 256;
	m_WideBuffer = new ThWchar[m_WideBufferSize];
	m_Enabled = true;
	m_PrefixFlags.Combine( ePrefix::Func );	
	AddOutputTarget( ThiLoggerOutputTargetPtr( new ThLoggerDebuggerOutput() ) );
}
//----------------------------------------------------------------------------------------
ThLogger::~ThLogger()
{
	if(m_Buffer)
		delete[] m_Buffer;

	if(m_WideBuffer)
		delete[] m_WideBuffer;
}
//----------------------------------------------------------------------------------------
ThFormatHelper& ThLogger::MakeFormatHelper(eMessageSeverity::Val severity, const ThI8* func, const ThI8* file, ThI32 line, const ThI8* format)
{
	ThFormatHelper& result = m_Helpers.local();

	if(!m_Enabled)
		return result;

	ThSize sz = 5 * 3 + 4 + 1 + 5 + 1;//5*3 for description, 5 for severity type, 4 for tag placeholder {%s}, 1 for end line, 9 for "-> [" format tag, 2 for "]"

	if( m_PrefixFlags.CheckFlag(ePrefix::File) )
	{
		sz += strlen(file);
	}

	if( m_PrefixFlags.CheckFlag(ePrefix::Line) )
	{
		sz += 9;
	}

	if( m_PrefixFlags.CheckFlag(ePrefix::Func) )
	{
		sz += strlen(func);
	}

	sz += strlen(format);

	if (result.m_FormatSize < sz)
	{
		result.m_FormatSize = sz;
		delete[] result.m_Format;
		result.m_Format = new ThI8[result.m_FormatSize];
	}

	result.m_WideFormat[0] = 0;
	result.m_Format[0] = 0;

	switch(severity)
	{
	case eMessageSeverity::Info:
		strcat(result.m_Format, "Inf: ");
		break;
	case eMessageSeverity::Warning:
		strcat(result.m_Format, "Wrn: ");
		break;
	case eMessageSeverity::Error:
		strcat(result.m_Format, "Err: ");
		break;
	case eMessageSeverity::Critical:
		strcat(result.m_Format, "Crt: ");
		break;
	}

	if( m_PrefixFlags.CheckFlag(ePrefix::File) )
	{
		strcat(result.m_Format, "Fl: ");
		strcat(result.m_Format, file);
		strcat(result.m_Format, " ");
	}

	if( m_PrefixFlags.CheckFlag(ePrefix::Line) )
	{
		strcat(result.m_Format, "Ln: ");
		ThI8 buf[16];
		ThStringUtilities::ToString(line, buf, 16);
		strcat(result.m_Format, buf);
		strcat(result.m_Format, " ");
	}

	if( m_PrefixFlags.CheckFlag(ePrefix::Func) )
	{
		strcat(result.m_Format, "Fn: ");
		strcat(result.m_Format, func);
		strcat(result.m_Format, " ");
	}

	strcat(result.m_Format, "-> [");
	strcat(result.m_Format, format);
	strcat(result.m_Format, "]");

	return result;
}
//----------------------------------------------------------------------------------------
ThFormatHelper& ThLogger::MakeFormatHelper(eMessageSeverity::Val severity, const ThI8* func, const ThI8* file, ThI32 line, const ThWchar* format)
{
	ThFormatHelper& result = m_Helpers.local();

	if(!m_Enabled)
		return result;

	ThWchar buf[128];

	ThSize sz = 5 * 3 + 4 + 1 + 5 + 1;//5*3 for description, 5 for severity type, 4 for tag placeholder {%s}, 1 for end line, 9 for "-> [" format tag, 2 for "]"

	if( m_PrefixFlags.CheckFlag(ePrefix::File) )
	{
		sz += strlen(file);
	}

	if( m_PrefixFlags.CheckFlag(ePrefix::Line) )
	{
		sz += 9;
	}

	if( m_PrefixFlags.CheckFlag(ePrefix::Func) )
	{
		sz += strlen(func);
	}

	sz += wcslen(format);

	if (result.m_WideFormatSize < sz)
	{
		result.m_WideFormatSize = sz;
		delete[] result.m_WideFormat;
		result.m_WideFormat = new ThWchar[result.m_WideFormatSize];
	}

	result.m_WideFormat[0] = 0;
	result.m_Format[0] = 0;

	switch(severity)
	{
	case eMessageSeverity::Info:
		wcscat(result.m_WideFormat, L"Inf: ");
		break;
	case eMessageSeverity::Warning:
		wcscat(result.m_WideFormat, L"Wrn: ");
		break;
	case eMessageSeverity::Error:
		wcscat(result.m_WideFormat, L"Err: ");
		break;
	case eMessageSeverity::Critical:
		wcscat(result.m_WideFormat, L"Crt: ");
		break;
	}

	if( m_PrefixFlags.CheckFlag(ePrefix::File) )
	{
		wcscat(result.m_WideFormat, L"Fl: ");
		ThStringUtilities::Utf8ToWideString(file, buf, 128);
		wcscat(result.m_WideFormat, buf);
		wcscat(result.m_WideFormat, L" ");
	}

	if( m_PrefixFlags.CheckFlag(ePrefix::Line) )
	{
		wcscat(result.m_WideFormat, L"Ln: ");
		ThStringUtilities::ToWideString(line, buf, 128);
		wcscat(result.m_WideFormat, buf);
		wcscat(result.m_WideFormat, L" ");
	}

	if( m_PrefixFlags.CheckFlag(ePrefix::Func) )
	{
		wcscat(result.m_WideFormat, L"Fn: ");
		ThStringUtilities::Utf8ToWideString(func, buf, 128);
		wcscat(result.m_WideFormat, buf);
		wcscat(result.m_WideFormat, L" ");
	}

	wcscat(result.m_WideFormat, L"-> [");
	wcscat(result.m_WideFormat, format);
	wcscat(result.m_WideFormat, L"]");

	return result;
}
//----------------------------------------------------------------------------------------
void ThLogger::AddIgnore(MessageId id)
{
	Mutex::scoped_lock lock(m_Mutex);

	m_IgnoreList.Insert(id);
}
//----------------------------------------------------------------------------------------
void ThLogger::AddIgnores(const IgnoreList& lst)
{			
	Mutex::scoped_lock lock(m_Mutex);

	for(IgnoreList::ConstIterator i = lst.Begin(); i != lst.End(); ++i)
		m_IgnoreList.Insert(i->Key());
}
//----------------------------------------------------------------------------------------
void ThLogger::RemoveIgnores(const IgnoreList& lst)
{
	Mutex::scoped_lock lock(m_Mutex);

	for(IgnoreList::ConstIterator i = lst.Begin(); i != lst.End(); ++i)
	{
		IgnoreList::Iterator j = m_IgnoreList.Find(i->Key());

		if(j != m_IgnoreList.End() )
			m_IgnoreList.Erase(j);
	}			
}
//----------------------------------------------------------------------------------------
void ThLogger::ClearIgnores()
{
	Mutex::scoped_lock lock(m_Mutex);
	m_IgnoreList.Clear();
}
//----------------------------------------------------------------------------------------
const ThLogger::IgnoreList& ThLogger::GetIgnoreList()const
{
	return m_IgnoreList;
}
//----------------------------------------------------------------------------------------
void ThLogger::SetEnabled(ThBool val)
{
	Mutex::scoped_lock lock(m_Mutex);
	m_Enabled = val;
}
//----------------------------------------------------------------------------------------
ThBool ThLogger::GetEnabled()const
{
	return m_Enabled;
}
//----------------------------------------------------------------------------------------
void ThLogger::Log(MessageId id, const ThI8* formatString, ...)
{
	va_list args;	
	va_start(args, formatString);

	Log(id, formatString, args);

	va_end(args);
}
//----------------------------------------------------------------------------------------
void ThLogger::Log(MessageId id, const ThWchar* formatString, ...)
{
	va_list args;	
	va_start(args, formatString);

	Log(id, formatString, args);

	va_end(args);
}
//----------------------------------------------------------------------------------------
void ThLogger::Log(MessageId id, const ThI8* formatString, va_list args)
{
	Mutex::scoped_lock lock(m_Mutex);

	//check if the logger is enabled
	if(!m_Enabled)
		return;

	//check if this message is not in the ignore list
	if(m_IgnoreList.Find(id) != m_IgnoreList.End())
		return;

	ThI32 written = vsnprintf(m_Buffer, m_BufferSize, formatString, args);

	//if the message is too short for the message, increase its size
	while(written == -1)
	{		
		m_BufferSize *= 2;

		if (m_BufferSize > 512)
		{
			strcpy(m_Buffer, "Message is too long or has invalid format.");
			//assert(0 && "Invalid log message.");
			break;
		}

		delete[] m_Buffer;
		m_Buffer = new ThI8[m_BufferSize];
		written = vsnprintf(m_Buffer, m_BufferSize, formatString, args);
	}	

	//write message tag
	ThU32 tagSize = strlen(id) + 3;//tag + {}\n
	if( (m_BufferSize - written) < tagSize )
	{
		m_BufferSize = m_BufferSize + tagSize + 1;
		ThI8* tmpStr = new ThI8[m_BufferSize];
		strcpy(tmpStr, m_Buffer);
		delete[]m_Buffer;
		m_Buffer = tmpStr;
	}

	strcat(m_Buffer, "{");
	strcat(m_Buffer, id);
	strcat(m_Buffer, "}\n");

	//output to the targets
	for(OutputTargets::Iterator i = m_OutputTargets.Begin(); i != m_OutputTargets.End(); ++i)
	{
		i->Value()->Print(m_Buffer);
	}
}
//----------------------------------------------------------------------------------------
void ThLogger::Log(MessageId id, const ThWchar* formatString, va_list args)
{
	Mutex::scoped_lock lock(m_Mutex);

	//check if the logger is enabled
	if(!m_Enabled)
		return;

	//check if this message is not in the ignore list
	if(m_IgnoreList.Find(id) != m_IgnoreList.End())
		return;

	ThI32 written = _vsnwprintf(m_WideBuffer, m_WideBufferSize, formatString, args);

	//if the message is too short for the message, increase its size
	while(written == -1)
	{		
		m_WideBufferSize *= 2;

		if (m_WideBufferSize > 512)
		{
			wcscpy(m_WideBuffer, L"Message is too long or has invalid format.");
			//assert(0 && "Invalid log message.");
			break;
		}

		delete[] m_WideBuffer;
		m_WideBuffer = new ThWchar[m_WideBufferSize];
		written = _vsnwprintf(m_WideBuffer, m_WideBufferSize, formatString, args);
	}	

	//write message tag
	ThU32 tagSize = strlen(id) + 3;//tag + {}\n
	if( (m_WideBufferSize - written) < tagSize )
	{
		m_WideBufferSize = m_WideBufferSize + tagSize + 1;
		ThWchar* tmpStr = new ThWchar[m_WideBufferSize];
		wcscpy(tmpStr, m_WideBuffer);
		delete[]m_WideBuffer;
		m_WideBuffer = tmpStr;
	}

	wcscat(m_WideBuffer, L"{");
	ThWchar buf[128];
	ThStringUtilities::Utf8ToWideString(id, buf, 128);
	wcscat(m_WideBuffer, buf);
	wcscat(m_WideBuffer, L"}\n");

	//output to the targets
	for(OutputTargets::Iterator i = m_OutputTargets.Begin(); i != m_OutputTargets.End(); ++i)
	{
		i->Value()->Print(m_WideBuffer);
	}
}
//----------------------------------------------------------------------------------------
bool ThLogger::AddOutputTarget( ThiLoggerOutputTargetPtr& tgt )
{
	return m_OutputTargets.Insert( tgt->GetType(), tgt);
}
//----------------------------------------------------------------------------------------
ThiLoggerOutputTargetPtr ThLogger::GetOutputTarget( ThiType* type )
{
	OutputTargets::Iterator i = m_OutputTargets.Find(type);

	if( i != m_OutputTargets.End() )
		return i->Value();
	else
		return ThiLoggerOutputTargetPtr();
}
//----------------------------------------------------------------------------------------
void ThLogger::SetPrefixFlags(ThFlags32& flags)
{
	m_PrefixFlags = flags;
}
//----------------------------------------------------------------------------------------
const ThFlags32& ThLogger::GetPrefixFlags()const
{
	return m_PrefixFlags;
}
//----------------------------------------------------------------------------------------
ThLogger& ThLogger::Instance()
{
	return Singleton<ThLogger>::Instance();
}

}//Thor