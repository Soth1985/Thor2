#pragma once
#include <Thor/Core/Common.h>
#include <dispatch/dispatch.h>
#include <functional>

namespace Thor
{
    struct eQueueType
    {
        enum Enum
        {
            Serial,
            Concurrent
        };
    };
    
    struct eQueuePriority
    {
        enum Enum
        {
            High,
            Default,
            Low,
            Background
        };
    };

    class ThDispatchSemaphore
    {
    public:
        ThDispatchSemaphore(ThI32 initialValue);
        ~ThDispatchSemaphore();
        
        void Wait(ThI64 timeoutNanoseconds = DISPATCH_TIME_FOREVER);
        void Signal();
    private:
        dispatch_semaphore_t m_Semaphore;
    };
    
    class ThDispatchGroup
    {
    public:
        ThDispatchGroup();
        ~ThDispatchGroup();
        void Wait(ThI64 timeoutNanoseconds);
        void Enter()const;
        void Leave()const;
        const dispatch_group_t& Id()const;
    private:
        ThDispatchGroup(const ThDispatchGroup& copy) = delete;
        ThDispatchGroup(ThDispatchGroup&& move) = delete;
        ThDispatchGroup& operator=(const ThDispatchGroup& rhs) = delete;
        ThDispatchGroup& operator=(ThDispatchGroup&& rhs) = delete;
        
        dispatch_group_t m_Group;
    };
    
    class ThDispatchQueue
    {
    public:
        ~ThDispatchQueue();
        ThDispatchQueue(eQueuePriority::Enum priority);
        ThDispatchQueue(const char* name, eQueueType::Enum type);
        
        using DispatchFunc = void(*)(void*);
        using DispatchApplyFunc = void(*)(void*, ThSize);
        
        void DispatchAsync(void* ctx, DispatchFunc func);
        void DispatchGroupAsync(const ThDispatchGroup& group, void* ctx, DispatchFunc func);
        void DispatchGroupNotify(const ThDispatchGroup& group, void* ctx, DispatchFunc func);
        void DispatchSync(void* ctx, DispatchFunc func);
        void DispatchApply(ThSize count, void* ctx, DispatchApplyFunc func);
        
    private:
        ThDispatchQueue(const ThDispatchQueue& copy) = delete;
        ThDispatchQueue(ThDispatchQueue&& move) = delete;
        ThDispatchQueue& operator=(const ThDispatchQueue& rhs) = delete;
        ThDispatchQueue& operator=(ThDispatchQueue&& rhs) = delete;
        
        dispatch_queue_t m_Queue;
    };
}
