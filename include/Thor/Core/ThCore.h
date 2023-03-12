#pragma once

namespace Thor{

class ThCore
{
private:
    
    static class ThiMemoryAllocator* m_SystemAllocator;
    static class ThLogger* m_Logger;
    static class ThUidGenerator* m_UidGenerator;
    
    ~ThCore() = delete;
    ThCore() = delete;
    ThCore(const ThCore& rhs) = delete;
    
public:

    static void Initialize();
    static void Shutdown();

    static ThiMemoryAllocator* GetSystemAllocator()
    {
        return m_SystemAllocator;
    }

    static ThLogger* GetLogger()
    {
        return m_Logger;
    }

    static ThUidGenerator* GetUidGenerator()
    {
        return m_UidGenerator;
    }
};

}
