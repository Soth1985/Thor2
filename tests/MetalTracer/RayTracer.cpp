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
            ThVec3f lowerLeftCorner(-2.0f, -1.0f, -1.0f);
            ThVec3f horizontal(4.0f / m_Options.m_Width, 0.0f, 0.0f);
            ThVec3f vertical(0.0f, 2.0f / m_Options.m_Height, 0.0f);
            ThVec3f origin;
            for (ThI32 j = m_Options.m_Height - 1; j >= 0; --j)
            {
                for (ThI32 i = 0; i < m_Options.m_Width; ++i)
                {
                    ThVec3f direction = lowerLeftCorner + i * horizontal + j * vertical;
                    direction.Normalize();
                    ThRayf ray(origin, direction);
                    ThVec3f color = TraceRay(ray);
                    this->m_Film->Pixel(i, j) = ThVec4ub(255.99f * color.r(), 255.99f * color.g(), 255.99f * color.b(), 255);
                }
            }
            this->m_FramesRendered++;
            this->m_State = RayTracerState::FrameReady;
        });
        return true;
    }
    
    return false;
}

ThVec3f RayTracer::TraceRay(const ThRayf& ray)
{
    ThSpheref sphere(ThVec3f(0.0f,0.0f,-1.0f), 0.5f);
    ThRayHitf hit;
    
    if (RayIntersectSphere(ray, sphere, 0.0, 1000.0, hit))
        return 0.5f * ThVec3f(hit.norm.x() + 1.0, hit.norm.y() + 1.0, hit.norm.z() + 1.0);
    
    float t = 0.5f * (ray.GetDirection().y() + 1.0f);
    ThVec3f result = t * ThVec3f(0.5f, 0.7f, 1.0f) + (1.0f - t) * ThVec3f(1.0f, 1.0f, 1.0f);
    return result;
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