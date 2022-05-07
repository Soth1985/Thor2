#include <Thor/Core/ThCore.h>
#include <Thor/Core/Debug/ThLogger.h>
#include <Thor/Core/Memory/ThSystemAllocator.h>

using namespace Thor;

ThLogger* ThCore::m_Logger = nullptr;
ThiMemoryAllocator* ThCore::m_SystemAllocator = nullptr;

void ThCore::Initialize()
{
    m_SystemAllocator = new ThSystemAllocator("Global");
    m_Logger = new ThLogger();
}

void ThCore::Shutdown()
{
    delete m_Logger;
    delete m_SystemAllocator;
}
