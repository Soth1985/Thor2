#pragma once

#include "Scene.h"
#include <atomic>
#include <random>

namespace Thor
{
    enum class RayTracerState : ThI32
    {
        Uninitialized,
        Idle,
        RenderingFrame,
        FrameReady
    };
    
    class Camera
    {
    public:
        Camera()
            :
        lowerLeftCorner(-2.0f, -1.0f, -1.0f),
        horizontal(4.0f, 0.0f, 0.0f),
        vertical(0.0f, 2.0f, 0.0f)
        {
            
        }
        
        ThRayf GetRay(float u, float v)
        {
            ThVec3f direction = lowerLeftCorner + u * horizontal + v * vertical - origin;
            direction.Normalize();
            return ThRayf(origin, direction);
        }
        
        ThVec3f lowerLeftCorner;
        ThVec3f horizontal;
        ThVec3f vertical;
        ThVec3f origin;
    };
    
    class Film
    {
    public:
        
        Film()
            :
        m_Width(0),
        m_Height(0),
        m_Frame(nullptr)
        {
            
        }
        
        void Init(ThI32 width, ThI32 height)
        {
            m_Width = width;
            m_Height = height;
            m_Frame = new ThVec4ub[width * height];
        }
        
        ~Film()
        {
            delete[] m_Frame;
        }
        
        const ThVec4ub& Pixel(int x, int y)const
        {
            return m_Frame[y * m_Width + x];
        }
        
        ThVec4ub& Pixel(int x, int y)
        {
            return m_Frame[y * m_Width + x];
        }
        
        const ThVec4ub* GetFrame()const
        {
            return m_Frame;
        }
    private:
        ThVec4ub* m_Frame;
        ThI32 m_Width;
        ThI32 m_Height;
    };
    
    struct RayTracerOptions
    {
        RayTracerOptions()
            :
        m_Width(0),
        m_Height(0),
        m_FramesToRender(1),
        m_SamplesPerPixel(1),
        m_TraceDepth(1)
        {
            
        }
        
        ThI32 m_Width;
        ThI32 m_Height;
        ThI32 m_FramesToRender;
        ThI32 m_SamplesPerPixel;
        ThI32 m_TraceDepth;
    };
    
    class RayTracer
    {
    public:
        RayTracer();
        ~RayTracer();
        void Init(const RayTracerOptions& options, Scene* scene);
        RayTracerState GetState();
        bool RenderFrame();
        const Film* GetFilm()const;
        const RayTracerOptions& GetOptions()const;
        void ResizeFilm(ThI32 width, ThI32 height);
        void FrameFetched();
    private:
        ThVec3f TraceScene(const ThRayf& ray, ThI32 depth);
        ThVec3f RandomPointOnSphere();
        bool ScatterLambert(const ComponentRef& mat, const ThRayf& rayIn, const ThRayHitf& hit, ThVec3f& attenuation, ThRayf& scattered);
        bool ScatterMetal(const ComponentRef& mat, const ThRayf& rayIn, const ThRayHitf& hit, ThVec3f& attenuation, ThRayf& scattered);
        bool ScatterDielectric(const ComponentRef& mat, const ThRayf& rayIn, const ThRayHitf& hit, ThVec3f& attenuation, ThRayf& scattered);
        float Schlick(float cosine, float n1, float n2);
        float Schlick2(const ThVec3f& vec, const ThVec3f& norm, float n1, float n2);
        std::atomic<RayTracerState> m_State;
        Film* m_Film;        
        ThI32 m_FramesRendered;
        Scene* m_Scene;
        std::mt19937 m_Generator;
        std::uniform_real_distribution<double> m_RngUniform;
        std::normal_distribution<double> m_RngNormal;
        RayTracerOptions m_Options;
        Camera m_Camera;
    };
}