#include "RayTracer.h"
#include <chrono>

using namespace Thor;

RayTracer::RayTracer()
    :
m_State(RayTracerState::Uninitialized),
m_FramesRendered(0),
m_Film(nullptr),
m_Scene(nullptr),
m_RngUniform(0.0, 1.0),
m_Queue(eQueuePriority::High)
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
        ThF32 aspect = ThF32(options.m_Width) / ThF32(options.m_Height);
        ThF32 focusDist = (options.m_CameraLookAt - options.m_CameraOrigin).Length();
        m_Camera.Init(options.m_CameraOrigin, options.m_CameraLookAt, options.m_CameraUp, options.m_CameraFov, aspect, options.m_CameraAperture, focusDist);
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
        
        float oneOverW = 1.0 / this->m_Options.m_Width;
        float oneOverH = 1.0f / this->m_Options.m_Height;
        for (ThI32 j = this->m_Options.m_Height - 1; j >= 0; --j)
        {
            m_Queue.DispatchGroupAsyncManual(m_Group, [=](void)
            {
                 for (ThI32 i = 0; i < this->m_Options.m_Width; ++i)
                 {
                     ThVec3f color;
                     for (ThI32 s = 0; s < this->m_Options.m_SamplesPerPixel; ++s)
                     {
                         float u = (i + this->m_RngUniform(m_Generator)) * oneOverW;
                         float v = (j + this->m_RngUniform(m_Generator)) * oneOverH;
                         ThRayf ray;
                         
                         if (this->m_Options.m_CameraMode == CameraMode::Normal)
                             ray = this->m_Camera.GetRay(u, v);
                         else
                             ray = this->m_Camera.GetRayLens(u, v);
                         
                         color += TraceScene(ray, 1);
                     }
                     color /= this->m_Options.m_SamplesPerPixel;
                     color.x() = Math::Sqrt(color.x());
                     color.y() = Math::Sqrt(color.y());
                     color.z() = Math::Sqrt(color.z());
                     this->m_Film->Pixel(i, j) = ThVec4ub(255.99f * color.r(), 255.99f * color.g(), 255.99f * color.b(), 255);
                 }
             });
        }
        
        m_Queue.DispatchGroupNotify(m_Group, [=](void)
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

ThVec3f RayTracer::TraceScene(const ThRayf& ray, ThI32 depth)
{
    ThRayHitf hitClosest;
    hitClosest.t = MAXFLOAT;
    ThI32 numHits = 0;
    ComponentRef* material = nullptr;
    for (ThSize i = 0; i < m_Scene->spheres.Size(); ++i)
    {
        ThRayHitf temp;
        
        if (RayIntersectSphere(ray, m_Scene->spheres[i].shape, 0.001, MAXFLOAT, temp))
        {
            ++numHits;
            if (temp.t < hitClosest.t)
            {
                material = &m_Scene->spheres[i].material;
                hitClosest = temp;
            }
        }
    }
    
    if (numHits > 0)
    {
        if (material)
        {
            switch (material->type)
            {
                case ComponentType::NormalMapMaterial:
                    return 0.5f * ThVec3f(hitClosest.norm.x() + 1.0, hitClosest.norm.y() + 1.0, hitClosest.norm.z() + 1.0);
                case ComponentType::LambertMaterial:
                case ComponentType::MetalMaterial:
                case ComponentType::DielectricMaterial:
                {
                    ThRayf scattered;
                    ThVec3f attenuation;
                    
                    if (depth > m_Options.m_TraceDepth)
                        return ThVec3f(0.0, 0.0, 0.0);
                    
                    bool didScatter = false;
                    
                    if (material->type == ComponentType::LambertMaterial)
                        didScatter = ScatterLambert(*material, ray, hitClosest, attenuation, scattered);
                    else if (material->type == ComponentType::MetalMaterial)
                        didScatter = ScatterMetal(*material, ray, hitClosest, attenuation, scattered);
                    else
                        didScatter = ScatterDielectric(*material, ray, hitClosest, attenuation, scattered);
                    
                    if (!didScatter)
                        return ThVec3f(0.0, 0.0, 0.0);
                    
                    return attenuation ^ TraceScene(scattered, depth + 1);
                }
                default:
                    return ThVec3f(0.0, 0.0, 0.0);
            }
        }
    }
    
    float t = 0.5f * (ray.GetDirection().y() + 1.0f);
    ThVec3f result = t * ThVec3f(0.5f, 0.7f, 1.0f) + (1.0f - t) * ThVec3f(1.0f, 1.0f, 1.0f);
    return result;
}

bool RayTracer::ScatterLambert(const ComponentRef& mat, const ThRayf& rayIn, const ThRayHitf& hit, ThVec3f& attenuation, ThRayf& scattered)
{
    ThVec3f dir = hit.norm + RandomPointOnSphere();
    scattered.SetOrigin(hit.pos);
    scattered.SetDirection(dir);
    attenuation = m_Scene->lamberts[mat.index].albedo;
    return true;
}

bool RayTracer::ScatterMetal(const ComponentRef& mat, const ThRayf& rayIn, const ThRayHitf& hit, ThVec3f& attenuation, ThRayf& scattered)
{
    ThVec3f rayInDir = rayIn.GetDirection();
    rayInDir.Normalize();
    ThVec3f reflected = Reflect(rayInDir, hit.norm);
    scattered.SetOrigin(hit.pos);
    scattered.SetDirection(reflected + m_Scene->metals[mat.index].fuzziness * RandomPointOnSphere());
    attenuation = m_Scene->metals[mat.index].albedo;
    return reflected * hit.norm > 0;
}

bool RayTracer::ScatterDielectric(const ComponentRef& mat, const ThRayf& rayIn, const ThRayHitf& hit, ThVec3f& attenuation, ThRayf& scattered)
{
    float n = m_Scene->dielectrics[mat.index].n;
    float n1 = 1;
    float n2 = n;
    ThVec3f outwardNormal;
    ThVec3f rayInDir = rayIn.GetDirection();
    rayInDir.Normalize();
    float dirDotN = rayInDir * hit.norm;
    float cosine = 0.0;
    float reflectProb = 0.0f;
    attenuation = ThVec3f(1.0, 1.0, 1.0);
    
    if (dirDotN > 0.0)
    {
        outwardNormal = -hit.norm;
        n1 = n;
        n2 = 1;
        cosine = n * dirDotN;
    }
    else
    {
        outwardNormal = hit.norm;
        cosine = - dirDotN;
    }
    
    ThVec3f refracted;
    if (Refract(rayInDir, outwardNormal, n1, n2, refracted))
    {
        reflectProb = Schlick(cosine, n1, n2);
        //reflectProb = Schlick2(rayInDir, outwardNormal, n1, n2);
        
        if (m_RngUniform(m_Generator) <= reflectProb)
        {
            ThVec3f reflected = Reflect(rayInDir, hit.norm);
            scattered = ThRayf(hit.pos, reflected);
        }
        else
            scattered = ThRayf(hit.pos, refracted);
            
        return true;
    }
    else
    {
        ThVec3f reflected = Reflect(rayInDir, hit.norm);
        scattered = ThRayf(hit.pos, reflected);
        return true;
    }
}

float RayTracer::Schlick(float cosine, float n1, float n2)
{
    float refIdx = n2 / n1;
    float r0 = (1.0 - refIdx) / (1.0 + refIdx);
    r0 *= r0;
    cosine = 1.0 - cosine;
    return r0 + (1.0 - r0) * cosine * cosine * cosine * cosine * cosine;
}

float RayTracer::Schlick2(const ThVec3f& vec, const ThVec3f& norm, float n1, float n2)
{
    float r0 = (n1 - n2) / (n1 + n2);
    r0 *= r0;
    float cosine = - vec * norm;
    
    if (n1 > n2)
    {
        float n = n1 / n2;
        float sinT2 = n * n * (1.0 - cosine * cosine);
        if (sinT2 > 1.0)
            return 1.0;
        cosine = Math::Sqrt(1.0 - sinT2);
    }
    cosine = 1.0 - cosine;
    return r0 + (1.0 - r0) * cosine * cosine * cosine * cosine * cosine;
}

ThVec3f RayTracer::RandomPointOnSphere()
{
    ThVec3f result;
    /*do
    {
        result.x() = m_RngUniform(m_Generator);
        result.y() = m_RngUniform(m_Generator);
        result.z() = m_RngUniform(m_Generator);
        
        result = 2.0 * result - ThVec3f(1.0,1.0,1.0);
    }
    while (result*result > 1.0);*/
    
    result.x() = m_RngUniform(m_Generator);
    result.y() = m_RngUniform(m_Generator);
    result.z() = m_RngUniform(m_Generator);
    result = 2.0 * result - ThVec3f(1.0,1.0,1.0);
    result.Normalize();
    
    return result;
}
