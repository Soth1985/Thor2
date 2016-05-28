#include <Thor/Core/Filesystem/LibFileSystem.h>

using namespace Thor;

int main()
{	
	ThLogger::Instance().AddIgnore("");	
	ThiFileSystemPtr fs = ThFileSystemFactory::Instance().CreateFileSystem( eFileSystemType::PhysFS );
	ThString p=fs->GetInitialPath();
	ThiFileStreamPtr file = fs->OpenFile("Log.txt", eStreamMode::WriteMode, eFileWriteMode::Truncate);
	ThiLoggerOutputTargetPtr foutput( new ThLoggerFileOutput(file) );
	ThLogger::Instance().AddOutputTarget(foutput);
	THOR_INF("Program has started, working directory = %s")("1", p.c_str() );
	THOR_INF("This is a message")("1");
	THOR_INF("Now quitting")("1");	
	return 0;
}