#include <Thor/Core/ThCore.h>
#include <Thor/Core/Debug/ThLogger.h>
#include <Thor/Core/Util/ThUidGenerator.h>
#include <Thor/Core/Memory/ThSystemAllocator.h>

using namespace Thor;

ThLogger* ThCore::m_Logger = nullptr;
ThiMemoryAllocator* ThCore::m_SystemAllocator = nullptr;
ThUidGenerator* ThCore::m_UidGenerator = nullptr;

void ThCore::Initialize()
{
    m_SystemAllocator = new ThSystemAllocator("Global");
    m_Logger = new ThLogger();
    m_UidGenerator = new ThUidGenerator();
}

void ThCore::Shutdown()
{
    delete m_Logger;
    delete m_SystemAllocator;
    delete m_UidGenerator;
}
