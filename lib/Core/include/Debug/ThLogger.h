#pragma once

#include <string>
#include <map>
#include <unordered_set>
#include <stdarg.h>
#include <tbb/combinable.h>
#include <Thor/Framework/ThiObject.h>
#include <Thor/Framework/Singleton.h>
#include <Thor/Framework/ThFlags.h>
#include <Thor/Framework/Containers/ThHashSet.h>
#include <Thor/Framework/Containers/ThHashMap.h>

#define THOR_INF(msg) Thor::ThLogger::Instance().MakeFormatHelper(eMessageSeverity::Info, __FUNCTION__, __FILE__, __LINE__, #msg)
#define THOR_WRN(msg) Thor::ThLogger::Instance().MakeFormatHelper(eMessageSeverity::Warning, __FUNCTION__, __FILE__, __LINE__, #msg)
#define THOR_ERR(msg) Thor::ThLogger::Instance().MakeFormatHelper(eMessageSeverity::Error, __FUNCTION__, __FILE__, __LINE__, #msg)
#define THOR_CRT(msg) Thor::ThLogger::Instance().MakeFormatHelper(eMessageSeverity::Critical, __FUNCTION__, __FILE__, __LINE__, #msg)

#define THOR_INF_W(msg) Thor::ThLogger::Instance().MakeFormatHelper(eMessageSeverity::Info, __FUNCTION__, __FILE__, __LINE__, L#msg)
#define THOR_WRN_W(msg) Thor::ThLogger::Instance().MakeFormatHelper(eMessageSeverity::Warning, __FUNCTION__, __FILE__, __LINE__, L#msg)
#define THOR_ERR_W(msg) Thor::ThLogger::Instance().MakeFormatHelper(eMessageSeverity::Error, __FUNCTION__, __FILE__, __LINE__, L#msg)
#define THOR_CRT_W(msg) Thor::ThLogger::Instance().MakeFormatHelper(eMessageSeverity::Critical, __FUNCTION__, __FILE__, __LINE__, L#msg)

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

THOR_DECLARE_CLASS_NS(ThiLoggerOutputTarget, Thor);
THOR_DECLARE_CLASS_NS(ThLoggerDebuggerOutput, Thor);
THOR_DECLARE_CLASS_NS(ThLoggerConsoleOutput, Thor);
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
class THOR_FRAMEWORK_DLL ThiLoggerOutputTarget: public ThiObject
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
	virtual void Print(ThI8* str) = 0;

	virtual void Print(ThWchar* str) = 0;
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
class THOR_FRAMEWORK_DLL ThLoggerDebuggerOutput: public ThiLoggerOutputTarget
{
public:
	/*!
	* \copydoc ThLoggerDebuggerOuput::Print
	*/
	virtual void Print(ThI8* str);

	virtual void Print(ThWchar* str);

	virtual ThiType* GetType()const;
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
class THOR_FRAMEWORK_DLL ThLoggerConsoleOutput: public ThiLoggerOutputTarget
{
public:
	/*!
	* \copydoc ThLoggerDebuggerOuput::Print
	*/
	virtual void Print(ThI8* str);

	virtual void Print(ThWchar* str);

	virtual ThiType* GetType()const;
};
//----------------------------------------------------------------------------------------
//
//					ThFormatHelper
//
//----------------------------------------------------------------------------------------
/*!
* \brief
* Intermediate class, which helps to decorate the message with additional information.
*
*/
struct THOR_FRAMEWORK_DLL ThFormatHelper
{
private:
	friend class ThLogger;

	ThI8* m_Format;
	ThSize m_FormatSize;

	ThWchar* m_WideFormat;
	ThSize m_WideFormatSize;

public:

	ThFormatHelper();
	~ThFormatHelper();

	void operator()(const ThI8* mid, ...);
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
class THOR_FRAMEWORK_DLL ThLogger:public NonCopyable
{
public:
	typedef const ThI8* MessageId;
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
	* \param format
	* Format string of the message.
	*
	*/
	ThFormatHelper& MakeFormatHelper(eMessageSeverity::Val severity, const ThI8* func, const ThI8* file, ThI32 line, const ThI8* format);

	ThFormatHelper& MakeFormatHelper(eMessageSeverity::Val severity, const ThI8* func, const ThI8* file, ThI32 line, const ThWchar* format);

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
	void SetEnabled(ThBool val);

	/*!
	* \brief
	* Queries the logging status.
	*
	* \returns
	* True if logging is enabled, false otherwise.
	*/
	ThBool GetEnabled()const;

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
	void Log(MessageId id, const ThI8* formatString, ...);

	void Log(MessageId id, const ThWchar* formatString, ...);

	/*!
	* \brief
	* Adds a new output target to the logger.
	*
	* \returns
	* True if target was added.
	*/
	bool AddOutputTarget( ThiLoggerOutputTargetPtr& tgt );

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
	ThiLoggerOutputTargetPtr GetOutputTarget( ThiType* type );

	static ThLogger& Instance();
	
private:		

	ThLogger();

	void Log(MessageId id, const ThI8* formatString, va_list args);

	void Log(MessageId id, const ThWchar* formatString, va_list args);

	friend struct ThFormatHelper;
	friend class Singleton<ThLogger>;
	typedef tbb::spin_mutex Mutex;
	typedef ThHashMap<ThiType*, ThiLoggerOutputTargetPtr> OutputTargets;
	
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
	ThI8*				m_Buffer;

	ThWchar*			m_WideBuffer;

	/*!
	* \brief
	* Size of the internal buffer.
	*/	
	ThU32				m_BufferSize;

	ThU32				m_WideBufferSize;

	/*!
	* \brief
	* Flags that determine how the message is to be decorated, see Thor::ePrefix.
	*/
	ThFlags32			m_PrefixFlags;		

	/*!
	* \brief
	* Determines if the logger is on or off.
	*/	
	ThBool				m_Enabled;

	/*!
	* \brief
	* Logger output targets.
	*/	
	OutputTargets		m_OutputTargets;

	tbb::combinable<ThFormatHelper> m_Helpers;
};	

}//Thor