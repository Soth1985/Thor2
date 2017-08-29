#pragma once

#include <atomic>

namespace Thor
{
    class SpinMutex
    {
    public:

        void lock() 
        {
            while (m_Flag.test_and_set(std::memory_order_acquire)) 
            {
                //spin
            }
        }

        void unlock() 
        {
            m_Flag.clear(std::memory_order_release);
        }
    private:
        std::atomic_flag m_Flag = ATOMIC_FLAG_INIT;
    };
}