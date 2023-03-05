#include <Thor/Core/Concurrent/ThDispatch.h>

using namespace Thor;

ThDispatchSemaphore::ThDispatchSemaphore(ThI32 initialValue)
{
    m_Semaphore = dispatch_semaphore_create(initialValue);
}

ThDispatchSemaphore::~ThDispatchSemaphore()
{
    
}

void ThDispatchSemaphore::Wait(ThI64 timeoutNanoseconds)
{
    dispatch_semaphore_wait(m_Semaphore, timeoutNanoseconds);
}

void ThDispatchSemaphore::Signal()
{
    dispatch_semaphore_signal(m_Semaphore);
}

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

void ThDispatchGroup::Enter()const
{
    dispatch_group_enter(m_Group);
}

void ThDispatchGroup::Leave()const
{
    dispatch_group_leave(m_Group);
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

void ThDispatchQueue::DispatchAsync(void* ctx, DispatchFunc func)
{
    dispatch_async_f(m_Queue, ctx, func);
}

void ThDispatchQueue::DispatchGroupAsync(const ThDispatchGroup& group, void* ctx, DispatchFunc func)
{
    dispatch_group_async_f(group.Id(), m_Queue, ctx, func);
}

void ThDispatchQueue::DispatchGroupNotify(const ThDispatchGroup& group, void* ctx, DispatchFunc func)
{
    dispatch_group_notify_f(group.Id(), m_Queue, ctx, func);
}

void ThDispatchQueue::DispatchSync(void* ctx, DispatchFunc func)
{
    dispatch_sync_f(m_Queue, ctx, func);
}

void ThDispatchQueue::DispatchApply(ThSize count, void* ctx, DispatchApplyFunc func)
{
    dispatch_apply_f(count, m_Queue, ctx, func);
}

void ThDispatchQueue::DispatchAsync(std::function<void()> func)
{
    dispatch_async(m_Queue, ^(void)
    {
        func();
    });
}

void ThDispatchQueue::DispatchGroupAsync(const ThDispatchGroup& group, std::function<void()> func)
{
    dispatch_group_async(group.Id(), m_Queue, ^(void)
    {
        func();
    });
}

void ThDispatchQueue::DispatchGroupAsyncManual(const ThDispatchGroup& group, std::function<void()> func)
{
    dispatch_group_t groupId = group.Id();
    dispatch_group_enter(groupId);
    dispatch_group_async(groupId, m_Queue, ^(void)
    {
        func();
        dispatch_group_leave(groupId);
    });
}

void ThDispatchQueue::DispatchGroupNotify(const ThDispatchGroup& group, std::function<void()> func)
{
    dispatch_group_notify(group.Id(), m_Queue, ^(void)
    {
        func();
    });
}

void ThDispatchQueue::DispatchSync(std::function<void()> func)
{
    dispatch_sync(m_Queue, ^(void)
    {
        func();
    });
}

void ThDispatchQueue::DispatchApply(ThSize count, std::function<void(ThSize)> func)
{
    dispatch_apply(count, m_Queue, ^(ThSize index)
    {
        func(index);
    });
}

void ThDispatchQueue::DispatchBarrierSync(std::function<void()> func)
{
    dispatch_barrier_sync(m_Queue, ^
    {
        func();
    });
}

void ThDispatchQueue::DispatchBarrierAsync(std::function<void()> func)
{
    dispatch_barrier_async(m_Queue, ^
    {
        func();
    });
}

void ThDispatchQueue::DispatchSuspend()
{
    dispatch_suspend(m_Queue);
}

void ThDispatchQueue::DispatchResume()
{
    dispatch_resume(m_Queue);
}

void ThDispatchQueue::DispatchBarrierSync(void* ctx, DispatchFunc func)
{
    dispatch_barrier_sync_f(m_Queue, ctx, func);
}

void ThDispatchQueue::DispatchBarrierAsync(void* ctx, DispatchFunc func)
{
    dispatch_barrier_async_f(m_Queue, ctx, func);
}
