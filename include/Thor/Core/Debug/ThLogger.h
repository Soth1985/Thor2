#pragma once

#include <string>
#include <stdarg.h>
#include <mutex>
#include <Thor/Core/Common.h>
#include <Thor/Core/ThFlags.h>
#include <Thor/Core/ThCore.h>
#include <Thor/Core/Debug/ThAssert.h>
#include <Thor/Core/Containers/ThVector.h>
#include <Thor/Core/Containers/ThHashSet.h>

//msvc should truncate comma if no extra params without ##__VA_ARGS__

#define THOR_INF(fmt, tag, ...) Thor::ThCore::GetLogger()->LogExtended(Thor::eMessageSeverity::Info, __FUNCTION__, __FILE__, __LINE__, fmt, tag, ##__VA_ARGS__)
#define THOR_WRN(fmt, tag, ...) Thor::ThCore::GetLogger()->LogExtended(Thor::eMessageSeverity::Warning, __FUNCTION__, __FILE__, __LINE__, fmt, tag, ##__VA_ARGS__)
#define THOR_ERR(fmt, tag, ...) Thor::ThCore::GetLogger()->LogExtended(Thor::eMessageSeverity::Error, __FUNCTION__, __FILE__, __LINE__, fmt, tag, ##__VA_ARGS__)
#define THOR_CRT(fmt, tag, ...) Thor::ThCore::GetLogger()->LogExtended(Thor::eMessageSeverity::Critical, __FUNCTION__, __FILE__, __LINE__, fmt, tag, ##__VA_ARGS__)

#define THOR_INF_W(fmt, tag, ...) Thor::ThCore::GetLogger()->LogExtended(Thor::eMessageSeverity::Info, __FUNCTION__, __FILE__, __LINE__, fmt, tag, ##__VA_ARGS__)
#define THOR_WRN_W(fmt, tag, ...) Thor::ThCore::GetLogger()->LogExtended(Thor::eMessageSeverity::Warning, __FUNCTION__, __FILE__, __LINE__, fmt, tag, ##__VA_ARGS__)
#define THOR_ERR_W(fmt, tag, ...) Thor::ThCore::GetLogger()->LogExtended(Thor::eMessageSeverity::Error, __FUNCTION__, __FILE__, __LINE__, fmt, tag, ##__VA_ARGS__)
#define THOR_CRT_W(fmt, tag, ...) Thor::ThCore::GetLogger()->LogExtended(Thor::eMessageSeverity::Critical, __FUNCTION__, __FILE__, __LINE__, fmt, tag, ##__VA_ARGS__)

namespace Thor{
//----------------------------------------------------------------------------------------
//
//					eMessageSeverity
//
//----------------------------------------------------------------------------------------
/*!
* \brief
* Enumeration, which declares the severity of a message being logged.
*
*/
struct eMessageSeverity
{
	enum Val
	{
		Info = 1 << 0,
		Warning = 1 << 1,
		Error = 1 << 2,
		Critical = 1 << 3,
	};
};
//----------------------------------------------------------------------------------------
//
//					ePrefix
//
//----------------------------------------------------------------------------------------
/*!
* \brief
* Enumeration, stating which additional information of the message (file name, line number, name of the function which invoked the logger) is to be printed.
*
*/
struct ePrefix
{
	enum Val
	{
		Func = 1 << 0,
		File = 1 << 1,
		Line = 1 << 2,
		All = Func | File | Line
	};
};

//----------------------------------------------------------------------------------------
//
//					ThiLoggerOutputTarget
//
//----------------------------------------------------------------------------------------
/*!
* \brief
* Base class for all of the logger`s output targets.
*
*/
class ThiLoggerOutputTarget
{
public:
	/*!
	* \brief
	* Print a message to the target.
	*
	* \param str
	* Message null-terminated string.
	*
	*/
	virtual void Print(ThChar* str) = 0;

	virtual void Print(ThWchar* str) = 0;
    
    virtual ~ThiLoggerOutputTarget();
};
//----------------------------------------------------------------------------------------
//
//					ThLoggerDebuggerOutput
//
//----------------------------------------------------------------------------------------
/*!
* \brief
* Output to the debugger`s console.
*
*/
class ThLoggerDebuggerOutput: public ThiLoggerOutputTarget
{
public:
	/*!
	* \copydoc ThLoggerDebuggerOuput::Print
	*/
	virtual void Print(ThChar* str) override;

	virtual void Print(ThWchar* str) override;
};
//----------------------------------------------------------------------------------------
//
//					ThLoggerConsoleOutput
//
//----------------------------------------------------------------------------------------
/*!
* \brief
* Output to the program`s console.
*
*/
class ThLoggerConsoleOutput: public ThiLoggerOutputTarget
{
public:
	/*!
	* \copydoc ThLoggerDebuggerOuput::Print
	*/
	virtual void Print(ThChar* str) override;

	virtual void Print(ThWchar* str) override;
};
//----------------------------------------------------------------------------------------
//
//					ThLogger
//
//----------------------------------------------------------------------------------------
/*!
* \brief
* Printf style logger.
*
* This logger is designed to be used by means of the following macros THOR_INF, THOR_WRN, THOR_ERR and THOR_CRT depending on the message severity.
* The first value in the macros parameter list should be a string tag id, identifying the type of the message. You can use this tags to filter out
* specific messages by using
* For example THOR_INF("Pawn [%s] is unable to find a path to Target [%s]")("AI", Pawn.Name(), Target.Name()).
* You can control which additional information the message is decorated with via setting appropriate prefix flags.
*
*/
class ThLogger
{
public:
	typedef const ThChar* MessageId;
	typedef ThHashSet<MessageId> IgnoreList;

	~ThLogger();

	/*!
	* \brief
	* Creates a decorator for the message.
	* 
	* \param severity
	* Message severity.
	* 
	* \param func
	* Name of the function that invoked the logger.
	*
	* \param file
	* Name of the source code file from which the logger was called.
	*
	* \param line
	* Source code line number of the call to the logger.
	*
	* \param formatString
	* Format string of the message.
	*
	*/
    void LogExtended(eMessageSeverity::Val severity, const ThChar* func, const ThChar* file, ThI32 line, const ThChar* formatString, MessageId id, ...);
    
    void LogExtended(eMessageSeverity::Val severity, const ThChar* func, const ThChar* file, ThI32 line, const ThWchar* formatString, MessageId id, ...);

	/*!
	* \brief
	* Ignore all messages with a specified id.
	*
	* \param id
	* Message id tag to add to ignore list.
	*
	*/
	void AddIgnore(MessageId id);

	/*!
	* \brief
	* Add a list of message ids to the ignore list.
	*
	* \param lst
	* List of message ids to add to ignore list.
	*
	*/
	void AddIgnores(const IgnoreList& lst);

	/*!
	* \brief
	* Removes specified message ids from the ignore list.
	*
	* \param lst
	* Message id tag to add to ignore list.
	*
	*/
	void RemoveIgnores(const IgnoreList& lst);

	/*!
	* \brief
	* Clear the ignore list.
	*
	*/
	void ClearIgnores();

	/*!
	* \brief
	* Queries the ignore list.
	*
	* \returns
	* Logger ignore list.
	*/
	const IgnoreList& GetIgnoreList()const;		

	/*!
	* \brief
	* Enable or disables logging.
	*
	* \param val
	* Boolean switch.
	*
	*/
	void SetEnabled(bool val);

	/*!
	* \brief
	* Queries the logging status.
	*
	* \returns
	* True if logging is enabled, false otherwise.
	*/
	bool GetEnabled()const;

	/*!
	* \brief
	* Sets the prefix flags.
	*
	* \param flags
	* flags to set.
	*/
	void SetPrefixFlags(ThFlags32& flags);

	/*!
	* \brief
	* Queries the prefix flags.
	*
	* \returns
	* Prefix flags.
	*/
	const ThFlags32& GetPrefixFlags()const;		

	/*!
	* \brief
	* Logs a message, uses a printf style format string.
	*
	* \param id
	* Message id.
	*
	* \param formatString
	* Message format string.
	*
	*/
	void Log(MessageId id, const ThChar* formatString, ...);

	void Log(MessageId id, const ThWchar* formatString, ...);

	/*!
	* \brief
	* Adds a new output target to the logger.
	*
	*/
	void AddOutputTarget(ThiLoggerOutputTarget* tgt);

	/*!
	* \brief
	* Adds a new output target to the logger.
	*
	* \param type
	* Type of the output target.
	*
	* \returns
	* Pointer to the target or null if the target is not registered with the logger.
	*/
	ThiLoggerOutputTarget* GetOutputTarget(ThSize index);
    
    static const ThChar* TagSystem;
	
private:		

	ThLogger();
	ThLogger(const ThLogger& rhs) = delete;

	void Log(MessageId id, const ThChar* formatString, va_list args);

	void Log(MessageId id, const ThWchar* formatString, va_list args);
    
    bool IsEnabled(MessageId id);

	friend struct ThFormatHelper;
	friend class ThCore;
	typedef std::mutex Mutex;
	typedef ThVector<ThiLoggerOutputTarget*> OutputTargets;
	
	/*!
	* \brief
	* List of message ids which are ignored.
	*/	
	IgnoreList		m_IgnoreList;

	/*!
	* \brief
	* Mutex for thread safety.
	*/	
    Mutex				m_Mutex;

	/*!
	* \brief
	* Internal buffer to print messages to.
	*/	
	ThChar*				m_Buffer;

	ThWchar*			m_WideBuffer;

	/*!
	* \brief
	* Size of the internal buffer.
	*/	
	ThU32				m_BufferSize;

	ThU32				m_WideBufferSize;
    
    ThI32               m_BufferOffset;

	/*!
	* \brief
	* Flags that determine how the message is to be decorated, see Thor::ePrefix.
	*/
	ThFlags32			m_PrefixFlags;		

	/*!
	* \brief
	* Determines if the logger is on or off.
	*/	
	bool				m_Enabled;

	/*!
	* \brief
	* Logger output targets.
	*/	
	OutputTargets		m_OutputTargets;
};	

}//Thor
