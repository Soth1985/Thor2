#include "RayTracer.h"
#include <chrono>
#include <dispatch/dispatch.h>

using namespace Thor;

void Scene::AddSphere(const ThSpheref& sphere)
{
    spheres.PushBack(sphere);
    objects.PushBack(Object());
    objects.Back().shape.type = ShapeType::Sphere;
    objects.Back().shape.index = spheres.Size() - 1;
}

ThVec3f Scene::TraceRay(const ThRayf& ray)
{
    ThRayHitf hitClosest;
    hitClosest.t = MAXFLOAT;
    ThI32 numHits = 0;
    for (ThSize i = 0; i < objects.Size(); ++i)
    {
        ThRayHitf temp;
        if (objects[i].shape.type == ShapeType::Sphere)
        {
            if (RayIntersectSphere(ray, spheres[objects[i].shape.index], 0.0, MAXFLOAT, temp))
            {
                ++numHits;
                if (temp.t < hitClosest.t)
                {                    
                    hitClosest = temp;
                }
            }
                
        }
    }
    
    if (numHits > 0)
        return 0.5f * ThVec3f(hitClosest.norm.x() + 1.0, hitClosest.norm.y() + 1.0, hitClosest.norm.z() + 1.0);
    
    float t = 0.5f * (ray.GetDirection().y() + 1.0f);
    ThVec3f result = t * ThVec3f(0.5f, 0.7f, 1.0f) + (1.0f - t) * ThVec3f(1.0f, 1.0f, 1.0f);
    return result;
}

RayTracer::RayTracer()
    :
m_State(RayTracerState::Uninitialized),
m_FramesRendered(0),
m_Film(nullptr),
m_Scene(nullptr),
m_Rng(0.0, 1.0)
{
    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
    m_Generator.seed(seed);
}

RayTracer::~RayTracer()
{
    if (m_Scene)
        delete m_Scene;
}

void RayTracer::Init(const RayTracerOptions& options, Scene* scene)
{
    if (m_State == RayTracerState::Uninitialized)
    {
        m_Options = options;
        m_Film = new Film();
        m_Film->Init(m_Options.m_Width, m_Options.m_Height);
        m_State = RayTracerState::Idle;
        m_Scene = scene;
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
            float oneOverW = 1.0 / m_Options.m_Width;
            float oneOverH = 1.0f / m_Options.m_Height;
            for (ThI32 j = m_Options.m_Height - 1; j >= 0; --j)
            {
                for (ThI32 i = 0; i < m_Options.m_Width; ++i)
                {
                    ThVec3f color;
                    for (ThI32 s = 0; s < m_Options.m_SamplesPerRay; ++s)
                    {
                        float u = (i + m_Rng(m_Generator)) * oneOverW;
                        float v = (j + m_Rng(m_Generator)) * oneOverH;
                        ThRayf ray = m_Camera.GetRay(u, v);
                        color += m_Scene->TraceRay(ray);
                    }
                    color /= m_Options.m_SamplesPerRay;
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