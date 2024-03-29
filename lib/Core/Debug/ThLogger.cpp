#include <Thor/Core/Debug/ThLogger.h>
#include <Thor/Core/String/ThStringUtilities.h>

#ifdef THOR_PLATFORM_WIN
#include <windows.h>
#endif

using namespace Thor;
    
const ThChar* ThLogger::TagSystem = "Core";
//----------------------------------------------------------------------------------------
//
//					ThLoggerDebuggerOutput
//
//----------------------------------------------------------------------------------------
void ThLoggerDebuggerOutput::Print(ThChar* str)
{
	//output into the debugger console	
#ifdef THOR_PLATFORM_WIN
	OutputDebugStringA(str);
#else
    printf("%s", str);
#endif
}

void ThLoggerDebuggerOutput::Print(ThWchar* str)
{
#ifdef THOR_PLATFORM_WIN
	OutputDebugStringW(str);
#else
    wprintf(L"%s", str);
#endif
}

ThiLoggerOutputTarget::~ThiLoggerOutputTarget()
{
}
//----------------------------------------------------------------------------------------
//
//					ThLoggerConsoleOutput
//
//----------------------------------------------------------------------------------------
void ThLoggerConsoleOutput::Print(ThChar* str)
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
//
//					ThLogger
//
//----------------------------------------------------------------------------------------
ThLogger::ThLogger()
{			
	m_BufferSize = 256;
	m_Buffer = new ThChar[m_BufferSize];
	m_WideBufferSize = 256;
	m_WideBuffer = new ThWchar[m_WideBufferSize];
	m_Enabled = true;
    m_BufferOffset = 0;
	m_PrefixFlags.Combine( ePrefix::Func );
    ThiLoggerOutputTarget* tgt = new ThLoggerDebuggerOutput();
	AddOutputTarget( tgt );
}
//----------------------------------------------------------------------------------------
ThLogger::~ThLogger()
{
    for (ThSize i = 0; i < m_OutputTargets.Size(); ++i)
    {
        delete m_OutputTargets[i];
    }
    
	if (m_Buffer)
		delete[] m_Buffer;

	if (m_WideBuffer)
		delete[] m_WideBuffer;
}
//----------------------------------------------------------------------------------------
void ThLogger::AddIgnore(MessageId id)
{
	std::lock_guard<std::mutex> lock(m_Mutex);

	m_IgnoreList.Insert(id);
}
//----------------------------------------------------------------------------------------
void ThLogger::AddIgnores(const IgnoreList& lst)
{			
	std::lock_guard<std::mutex> lock(m_Mutex);

	for(IgnoreList::ConstIterator i = lst.Begin(); i != lst.End(); ++i)
		m_IgnoreList.Insert(i->Key());
}
//----------------------------------------------------------------------------------------
void ThLogger::RemoveIgnores(const IgnoreList& lst)
{
	std::lock_guard<std::mutex> lock(m_Mutex);

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
	std::lock_guard<std::mutex> lock(m_Mutex);
	m_IgnoreList.Clear();
}
//----------------------------------------------------------------------------------------
const ThLogger::IgnoreList& ThLogger::GetIgnoreList()const
{
	return m_IgnoreList;
}
//----------------------------------------------------------------------------------------
void ThLogger::SetEnabled(bool val)
{
	std::lock_guard<std::mutex> lock(m_Mutex);
	m_Enabled = val;
}
//----------------------------------------------------------------------------------------
bool ThLogger::GetEnabled()const
{
	return m_Enabled;
}
//----------------------------------------------------------------------------------------
void ThLogger::Log(MessageId id, const ThChar* formatString, ...)
{
    if (!IsEnabled(id))
        return;
    
    std::lock_guard<std::mutex> lock(m_Mutex);
    m_Buffer[0] = 0;
	va_list args;	
	va_start(args, formatString);

	Log(id, formatString, args);

	va_end(args);
}
//----------------------------------------------------------------------------------------
void ThLogger::Log(MessageId id, const ThWchar* formatString, ...)
{
    if (!IsEnabled(id))
        return;
    
    m_WideBuffer[0] = 0;
    std::lock_guard<std::mutex> lock(m_Mutex);
	va_list args;	
	va_start(args, formatString);

	Log(id, formatString, args);

	va_end(args);
}
//----------------------------------------------------------------------------------------
bool ThLogger::IsEnabled(MessageId id)
{
    //check if the logger is enabled
    if(!m_Enabled)
        return false;
    
    //check if this message is not in the ignore list
    if(m_IgnoreList.Find(id) != m_IgnoreList.End())
        return false;
    
    return true;
}
    //----------------------------------------------------------------------------------------
void ThLogger::Log(MessageId id, const ThChar* formatString, va_list args)
{
    ThChar* buffer = m_Buffer + m_BufferOffset;
    ThI32 bufferSize = m_BufferSize - m_BufferOffset;// * sizeof(ThI8);
    va_list temp;
    va_copy(temp, args);
	ThI32 written = vsnprintf(buffer, bufferSize, formatString, temp);
    va_end(temp);
    
    if (written == -1)
    {
        strcpy(buffer, "Encoding error");
        THOR_ASSERT(0, "Encoding error");
        return;
    }

	//if the message is too short for the message, increase its size
	while(written >= bufferSize)
	{		
		m_BufferSize *= 2;

		if (m_BufferSize > 1024)
		{
            m_Buffer[0] = 0;
			strcpy(m_Buffer, "Message is too long or has invalid format.");
			//assert(0 && "Invalid log message.");
			break;
		}
        
        bool formatAliases = false;
        
        if (formatString >= m_Buffer && formatString < (m_Buffer + m_BufferSize))
            formatAliases = true;
        
        ThChar* newBuffer = new ThChar[m_BufferSize];
        strcpy(newBuffer, m_Buffer);
		delete[] m_Buffer;
		m_Buffer = newBuffer;
        
        if (formatAliases)
            formatString = m_Buffer;
        
        buffer = m_Buffer + m_BufferOffset;
        bufferSize = m_BufferSize - m_BufferOffset;// * sizeof(ThI8);
		
        va_list temp;
        va_copy(temp, args);
        written = vsnprintf(buffer, bufferSize, formatString, args);
        va_end(temp);
	}	

	//write message tag
	ThU32 tagSize = strlen(id) + 3;//tag + {}\n
	if( (bufferSize - written) > tagSize )
	{
        strcat(buffer, "{");
        strcat(buffer, id);
        strcat(buffer, "}\n");
	}

	//output to the targets
	for (OutputTargets::Iterator i = m_OutputTargets.Begin(); i != m_OutputTargets.End(); ++i)
	{
        (*i)->Print(buffer);
	}
}
//----------------------------------------------------------------------------------------
void ThLogger::Log(MessageId id, const ThWchar* formatString, va_list args)
{
    ThWchar* buffer = m_WideBuffer + m_BufferOffset;
    ThI32 bufferSize = m_WideBufferSize - m_BufferOffset;// * sizeof(ThWchar);
    va_list temp;
    va_copy(temp, args);
    ThI32 written = std::swprintf(buffer, bufferSize, formatString, temp);
    va_end(temp);

	//if the message is too short for the message, increase its size
	while(written == -1)
	{		
		m_WideBufferSize *= 2;

		if (m_WideBufferSize > 1024)
		{
			wcscpy(m_WideBuffer, L"Message is too long or has invalid format.");
			//assert(0 && "Invalid log message.");
			break;
		}
        
        bool formatAliases = false;
        
        if (formatString >= m_WideBuffer && formatString < (m_WideBuffer + m_BufferSize))
            formatAliases = true;
        
        ThWchar* newBuffer = new ThWchar[m_WideBufferSize];
        wcscpy(newBuffer, m_WideBuffer);
        delete[] m_WideBuffer;
        m_WideBuffer = newBuffer;
        
        if (formatAliases)
            formatString = m_WideBuffer;
        
        buffer = m_WideBuffer + m_BufferOffset;
        bufferSize = m_WideBufferSize - m_BufferOffset;// * sizeof(ThWchar);
        va_list temp;
        va_copy(temp, args);
        written = std::swprintf(buffer, bufferSize, formatString, temp);
        va_end(temp);
	}	

	//write message tag
	ThU32 tagSize = strlen(id) + 3;//tag + {}\n
	if( (m_WideBufferSize - written) > tagSize )
	{
        wcscat(m_WideBuffer, L"{");
        ThWchar buf[128];
        ThStringUtilities::Utf8ToWideString(id, buf, 128);
        wcscat(m_WideBuffer, buf);
        wcscat(m_WideBuffer, L"}\n");
    }	

	//output to the targets
	for(OutputTargets::Iterator i = m_OutputTargets.Begin(); i != m_OutputTargets.End(); ++i)
	{
        (*i)->Print(buffer);
	}
}
//----------------------------------------------------------------------------------------
void ThLogger::LogExtended(eMessageSeverity::Val severity, const ThChar* func, const ThChar* file, ThI32 line, const ThChar* formatString, MessageId id, ...)
{
    if (!IsEnabled(id))
        return;
    
    std::lock_guard<std::mutex> lock(m_Mutex);
    
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
    
    sz += strlen(formatString);
    
    if (m_BufferSize < sz)
    {
        m_BufferSize = sz;
        delete[] m_Buffer;
        m_Buffer = new ThChar[m_BufferSize];
    }
    
    m_Buffer[0] = 0;
    
    switch(severity)
    {
        case eMessageSeverity::Info:
            strcat(m_Buffer, "Inf: ");
            break;
        case eMessageSeverity::Warning:
            strcat(m_Buffer, "Wrn: ");
            break;
        case eMessageSeverity::Error:
            strcat(m_Buffer, "Err: ");
            break;
        case eMessageSeverity::Critical:
            strcat(m_Buffer, "Crt: ");
            break;
    }
    
    if( m_PrefixFlags.CheckFlag(ePrefix::File) )
    {
        strcat(m_Buffer, "Fl: ");
        strcat(m_Buffer, file);
        strcat(m_Buffer, " ");
    }
    
    if( m_PrefixFlags.CheckFlag(ePrefix::Line) )
    {
        strcat(m_Buffer, "Ln: ");
        ThChar buf[16];
        ThStringUtilities::ToString(line, buf, 16);
        strcat(m_Buffer, buf);
        strcat(m_Buffer, " ");
    }
    
    if( m_PrefixFlags.CheckFlag(ePrefix::Func) )
    {
        strcat(m_Buffer, "Fn: ");
        strcat(m_Buffer, func);
        strcat(m_Buffer, " ");
    }
    
    strcat(m_Buffer, "-> [");
    strcat(m_Buffer, formatString);
    strcat(m_Buffer, "]");
    
    va_list args;
    va_start(args, id);
    
    m_BufferOffset = strlen(m_Buffer) + 1;
    Log(id, m_Buffer, args);
    m_BufferOffset = 0;
    
    va_end(args);
}
    //----------------------------------------------------------------------------------------
void ThLogger::LogExtended(eMessageSeverity::Val severity, const ThChar* func, const ThChar* file, ThI32 line, const ThWchar* formatString, MessageId id, ...)
{
    if (!IsEnabled(id))
        return;
    
    std::lock_guard<std::mutex> lock(m_Mutex);
    
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
    
    sz += wcslen(formatString);
    
    if (m_WideBufferSize < sz)
    {
        m_WideBufferSize = sz;
        delete[] m_WideBuffer;
        m_WideBuffer = new ThWchar[m_WideBufferSize];
    }
    
    m_WideBuffer[0] = 0;
    
    switch(severity)
    {
        case eMessageSeverity::Info:
            wcscat(m_WideBuffer, L"Inf: ");
            break;
        case eMessageSeverity::Warning:
            wcscat(m_WideBuffer, L"Wrn: ");
            break;
        case eMessageSeverity::Error:
            wcscat(m_WideBuffer, L"Err: ");
            break;
        case eMessageSeverity::Critical:
            wcscat(m_WideBuffer, L"Crt: ");
            break;
    }
    
    if( m_PrefixFlags.CheckFlag(ePrefix::File) )
    {
        wcscat(m_WideBuffer, L"Fl: ");
        ThStringUtilities::Utf8ToWideString(file, buf, 128);
        wcscat(m_WideBuffer, buf);
        wcscat(m_WideBuffer, L" ");
    }
    
    if( m_PrefixFlags.CheckFlag(ePrefix::Line) )
    {
        wcscat(m_WideBuffer, L"Ln: ");
        ThStringUtilities::ToWideString(line, buf, 128);
        wcscat(m_WideBuffer, buf);
        wcscat(m_WideBuffer, L" ");
    }
    
    if( m_PrefixFlags.CheckFlag(ePrefix::Func) )
    {
        wcscat(m_WideBuffer, L"Fn: ");
        ThStringUtilities::Utf8ToWideString(func, buf, 128);
        wcscat(m_WideBuffer, buf);
        wcscat(m_WideBuffer, L" ");
    }
    
    wcscat(m_WideBuffer, L"-> [");
    wcscat(m_WideBuffer, formatString);
    wcscat(m_WideBuffer, L"]");
    
    va_list args;
    va_start(args, id);
    
    m_BufferOffset = wcslen(m_WideBuffer) + 1;
    Log(id, m_WideBuffer, args);
    m_BufferOffset = 0;
    
    va_end(args);
}
//----------------------------------------------------------------------------------------
void ThLogger::AddOutputTarget( ThiLoggerOutputTarget* tgt )
{
    return m_OutputTargets.PushBack(tgt);
}
//----------------------------------------------------------------------------------------
ThiLoggerOutputTarget* ThLogger::GetOutputTarget( ThSize index )
{
    return m_OutputTargets[index];
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
