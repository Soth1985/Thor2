#include "RayTracer.h"
#include <dispatch/dispatch.h>

using namespace Thor;

RayTracer::RayTracer()
    :
m_State(RayTracerState::Uninitialized),
m_FramesRendered(0),
m_Film(nullptr)
{
    
}

void RayTracer::Init(const RayTracerOptions& options)
{
    if (m_State == RayTracerState::Uninitialized)
    {
        m_Options = options;
        m_Film = new Film();
        m_Film->Init(m_Options.m_Width, m_Options.m_Height);
        m_State = RayTracerState::Idle;
    }
}

RayTracerState RayTracer::GetState()
{
    return m_State;
}

bool RayTracer::RenderFrame()
{
    if (m_State == RayTracerState::Uninitialized)
        return false;
    
    if ((m_Options.m_FramesToRender > 0) && (m_FramesRendered >= m_Options.m_FramesToRender))
        return false;
    
    if (m_State != RayTracerState::RenderingFrame)
    {
        m_State = RayTracerState::RenderingFrame;
        dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0);
        
        dispatch_async(queue,
        ^(void)
        {
            this->m_FramesRendered++;
            this->m_State = RayTracerState::FrameReady;
        });
        return true;
    }
    
    return false;
}

const Film* RayTracer::GetFilm()const
{
    return m_Film;
}

const RayTracerOptions& RayTracer::GetOptions()const
{
    return m_Options;
}

void RayTracer::FrameFetched()
{
    if (m_State == RayTracerState::FrameReady)
        m_State = RayTracerState::Idle;
}

void RayTracer::ResizeFilm(ThI32 width, ThI32 height)
{
    if (m_State == RayTracerState::Idle)
    {
        if (m_Film)
            delete m_Film;
        m_Film = new Film();
        m_Film->Init(width, height);
        m_Options.m_Width = width;
        m_Options.m_Height = height;
    }
}