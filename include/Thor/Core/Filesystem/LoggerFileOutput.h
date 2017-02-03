#ifndef THOR_SYSTEM_LOGGER_FILE_OUTPUT_H
#define THOR_SYSTEM_LOGGER_FILE_OUTPUT_H

#include <Thor/Core/Filesystem/ThiFileStream.h>
#include <Thor/Core/Debug/ThLogger.h>

namespace Thor
{
//----------------------------------------------------------------------------------------
//
//					ThLoggerFileOutput
//
//----------------------------------------------------------------------------------------
/*!
* \brief
* Output to the file.
*
*/
class THOR_FRAMEWORK_DLL ThLoggerFileOutput: public ThiLoggerOutputTarget
{
public:
	ThLoggerFileOutput();
	ThLoggerFileOutput(ThiFileStreamPtr& file);
	/*!
	* \copydoc ThLoggerDebuggerOuput::Print
	*/
	virtual void Print(ThChar* str);
	virtual void Print(ThWchar* str);
	virtual ThiType* GetType()const;
	void SetFile(ThiFileStreamPtr& file);

private:
	ThiFileStreamPtr m_File;
};

}//Thor

#endif
