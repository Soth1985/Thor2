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
        
        void DispatchAsync(std::function<void()>&& func);
        void DispatchGroupAsync(const ThDispatchGroup& group, std::function<void()>&& func);
        void DispatchGroupNotify(const ThDispatchGroup& group, std::function<void()>&& func);
        void DispatchSync(std::function<void()>&& func);
        void DispatchApply(ThSize count, std::function<void(ThSize)>&& func);
        
    private:
        ThDispatchQueue(const ThDispatchQueue& copy) = delete;
        ThDispatchQueue(ThDispatchQueue&& move) = delete;
        ThDispatchQueue& operator=(const ThDispatchQueue& rhs) = delete;
        ThDispatchQueue& operator=(ThDispatchQueue&& rhs) = delete;
        
        dispatch_queue_t m_Queue;
    };
}
