#include <Thor/Core/Concurrent/ThDispatch.h>

using namespace Thor;

ThDispatchGroup::~ThDispatchGroup()
{
    dispatch_release(m_Group);
}

ThDispatchGroup::ThDispatchGroup()
{
    m_Group = dispatch_group_create();
}

const dispatch_group_t& ThDispatchGroup::Id()const
{
    return m_Group;
}

void ThDispatchGroup::Wait(ThI64 timeoutNanoseconds)
{
    if (timeoutNanoseconds > 0)
    {
        dispatch_time_t timeout = dispatch_time(DISPATCH_TIME_NOW, timeoutNanoseconds);
        dispatch_group_wait(m_Group, timeout);
    }
    else
        dispatch_group_wait(m_Group, DISPATCH_TIME_FOREVER);
}

ThDispatchQueue::~ThDispatchQueue()
{
    dispatch_release(m_Queue);
}

ThDispatchQueue::ThDispatchQueue(eQueuePriority::Enum priority)
{
    long identifier = DISPATCH_QUEUE_PRIORITY_DEFAULT;
    
    if (priority == eQueuePriority::High)
        identifier = DISPATCH_QUEUE_PRIORITY_HIGH;
    else if(priority == eQueuePriority::Low)
        identifier = DISPATCH_QUEUE_PRIORITY_LOW;
    else if (priority == eQueuePriority::Background)
        identifier = DISPATCH_QUEUE_PRIORITY_BACKGROUND;

    m_Queue = dispatch_get_global_queue(identifier, 0);
}

ThDispatchQueue::ThDispatchQueue(const char* name, eQueueType::Enum type)
{
    dispatch_queue_attr_t attr = DISPATCH_QUEUE_SERIAL;
    
    if (type == eQueueType::Concurrent)
        attr = DISPATCH_QUEUE_CONCURRENT;
    
    m_Queue = dispatch_queue_create(name, attr);
}

void ThDispatchQueue::DispatchAsync(std::function<void()> func)
{
    dispatch_async(m_Queue, ^(void){
        func();
    });
}

void ThDispatchQueue::DispatchGroupAsync(const ThDispatchGroup& group, std::function<void()> func)
{
    dispatch_group_async(group.Id(), m_Queue, ^(void) {
        func();
    });
}

void ThDispatchQueue::DispatchSync(std::function<void()> func)
{
    dispatch_sync(m_Queue, ^(void){
        func();
    });
}

void ThDispatchQueue::DispatchApply(ThSize count, std::function<void(ThSize)> func)
{
    dispatch_apply(count, m_Queue, ^(ThSize index){
        func(index);
    });
}
