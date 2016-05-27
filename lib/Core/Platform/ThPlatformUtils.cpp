#include <Thor/Core/Platform/ThPlatformUtils.h>

#ifdef THOR_MS_WIN
	#include <windows.h>
	#include <dbghelp.h>
	#include <Thor/Framework/ThWinSystemWindow.h>
#endif

namespace Thor
{

static eMinidumpLevel::Val s_MinidupmLvl = eMinidumpLevel::High;
//----------------------------------------------------------------------------------------
//
//					ThPlatformUtils
//
//----------------------------------------------------------------------------------------
#ifdef THOR_MS_WIN

void ThPlatformUtils::DispatchOperatingSystemMessages()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
}
//----------------------------------------------------------------------------------------
ThiSystemWindowPtr ThPlatformUtils::CreateSystemWindow()
{
	return new ThWinSystemWindow();
}
//----------------------------------------------------------------------------------------
bool IsDataSectionNeeded( const WCHAR* pModuleName ) 
{
	
	// Check parameters
	if( pModuleName == 0 ) 
	{		
		return false; 
	}
	
	WCHAR curDir[MAX_PATH] = L"";
	GetCurrentDirectoryW(MAX_PATH-1, curDir);
	wcscat(curDir, L"\\");

	// Extract the module name 
	WCHAR drive[_MAX_DRIVE] = L"";
	WCHAR dir[_MAX_DIR]=L"";
	WCHAR fileName[_MAX_FNAME]=L"";
	_wsplitpath( pModuleName, drive, dir, fileName, 0 );

	WCHAR modulePath[_MAX_DRIVE + _MAX_DIR]=L"";
	wcscat(modulePath, drive);	
	wcscat(modulePath, dir);	

	// Compare the name with the list of known names and decide 	
	if( wcsicmp( curDir, modulePath ) == 0 ) 
	{		
		return true; 
	}
	else if( wcsicmp( fileName, L"ntdll" ) == 0 ) 
	{
		return true; 
	}	

	return false;
}
//----------------------------------------------------------------------------------------
BOOL CALLBACK MiniDumpCallback(
	PVOID                            pParam, 
	const PMINIDUMP_CALLBACK_INPUT   pInput, 
	PMINIDUMP_CALLBACK_OUTPUT        pOutput 
	) 
{
	BOOL bRet = FALSE; 


	// Check parameters 

	if( pInput == 0 ) 
		return FALSE; 

	if( pOutput == 0 ) 
		return FALSE; 


	// Process the callbacks 

	switch( pInput->CallbackType ) 
	{
	case IncludeModuleCallback: 
		{
			// Include the module into the dump 
			bRet = TRUE; 
		}
		break; 

	case IncludeThreadCallback: 
		{
			// Include the thread into the dump 
			bRet = TRUE; 
		}
		break; 

	case ModuleCallback: 
		{
			if(s_MinidupmLvl == eMinidumpLevel::Low)
			{
				// Does the module have ModuleReferencedByMemory flag set ?
				if( !(pOutput->ModuleWriteFlags & ModuleReferencedByMemory) ) 
				{
					// No, it does not - exclude it					
					pOutput->ModuleWriteFlags &= (~ModuleWriteModule); 
				}
			}

			if(s_MinidupmLvl == eMinidumpLevel::Med)
			{
				// Are data sections available for this module ?
				if( pOutput->ModuleWriteFlags & ModuleWriteDataSeg ) 
				{
					// Yes, they are, but do we need them?
					if( !IsDataSectionNeeded( pInput->Module.FullPath ) ) 
					{						
						pOutput->ModuleWriteFlags &= (~ModuleWriteDataSeg); 
					}
				}
			}

			bRet = TRUE; 
		}
		break; 

	case ThreadCallback: 
		{
			// Include all thread information into the minidump 
			bRet = TRUE;  
		}
		break; 

	case ThreadExCallback: 
		{
			// Include this information 
			bRet = TRUE;  
		}
		break; 

	case MemoryCallback: 
		{
			// We do not include any information here -> return FALSE 
			bRet = FALSE; 
		}
		break; 

	case CancelCallback: 
		break; 
	}

	return bRet; 

}
//----------------------------------------------------------------------------------------
LONG WINAPI OnApplicationCrushed(EXCEPTION_POINTERS* pep)
{
	ThString fileName("Dump");
	fileName+= "_" + s_AssemblyInfo.ToString();
	SYSTEMTIME time;
	GetSystemTime(&time);
	char buf[128]="";
	sprintf(buf, "_%d.%d.%d.%d.dmp", time.wDayOfWeek, time.wHour, time.wMinute, time.wMilliseconds);
	fileName += buf;
	HANDLE hFile = CreateFile( fileName.c_str(), GENERIC_READ | GENERIC_WRITE, 
		0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ); 

	if( ( hFile != NULL ) && ( hFile != INVALID_HANDLE_VALUE ) ) 
	{
		// Create the minidump 

		MINIDUMP_EXCEPTION_INFORMATION mdei; 

		mdei.ThreadId           = GetCurrentThreadId(); 
		mdei.ExceptionPointers  = pep; 
		mdei.ClientPointers     = FALSE;		

		MINIDUMP_TYPE mdt       = (MINIDUMP_TYPE)(MiniDumpNormal);

		if(s_MinidupmLvl == eMinidumpLevel::Low)
		{
			mdt = (MINIDUMP_TYPE)(MiniDumpWithIndirectlyReferencedMemory | MiniDumpScanMemory);
		}

		if(s_MinidupmLvl == eMinidumpLevel::Med)
		{
			mdt = (MINIDUMP_TYPE)(MiniDumpWithDataSegs | 
				MiniDumpWithHandleData |
				MiniDumpWithFullMemoryInfo | 
				MiniDumpWithThreadInfo | 
				MiniDumpWithUnloadedModules );
		}

		if(s_MinidupmLvl == eMinidumpLevel::High)
		{
			mdt = (MINIDUMP_TYPE)(MiniDumpWithFullMemory | 
				MiniDumpWithFullMemoryInfo | 
				MiniDumpWithHandleData | 
				MiniDumpWithThreadInfo | 
				MiniDumpWithUnloadedModules ); 
		}

		if(s_MinidupmLvl != eMinidumpLevel::High)
		{
			MINIDUMP_CALLBACK_INFORMATION mci; 

			mci.CallbackRoutine     = (MINIDUMP_CALLBACK_ROUTINE)MiniDumpCallback; 
			mci.CallbackParam       = 0; 
			BOOL rv = MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), 
				hFile, mdt, (pep != 0) ? &mdei : 0, 0, &mci );
		}
		else
		{
			BOOL rv = MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), 
				hFile, mdt, (pep != 0) ? &mdei : 0, 0, 0 );
		}		

		CloseHandle( hFile );
	}

	return EXCEPTION_EXECUTE_HANDLER;
}
//----------------------------------------------------------------------------------------
void ThPlatformUtils::SetupMinidump(eMinidumpLevel::Val lvl)
{
	s_MinidupmLvl = lvl;
	SetUnhandledExceptionFilter( &OnApplicationCrushed );
}

#endif//THOR_MS_WIN

}