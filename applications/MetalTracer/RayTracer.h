#pragma once

#include "Scene.h"
#include <Thor/Core/Concurrent/ThDispatch.h>
#include <atomic>
#include <random>
#include <chrono>
#include <functional>

namespace Thor
{
    template<class T>
    T UniformDistribution(T min, T max)
    {
        static /*thread_local*/ std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_real_distribution<double> distribution(min, max);
        return distribution(generator);
    }
    
    enum class CameraMode
    {
        Normal,
        LensDefocus
    };
    
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
        vertical(0.0f, 2.0f, 0.0f),
        lensRadius(1.0f)
        {
            
        }
        
        void Init(const ThVec3f& origin, const ThVec3f& lookAt, const ThVec3f& up, ThF32 vFov, ThF32 aspect, ThF32 aperture, ThF32 focusDist)
        {
            lensRadius = 0.5 * aperture;
            ThF32 theta = Math::DegToRad(vFov);
            ThF32 halfHeight = Math::Tan(0.5 * theta);
            ThF32 halfWidth = aspect * halfHeight;
            this->origin = origin;
            w = origin - lookAt;
            w.Normalize();
            u = up % w;
            u.Normalize();
            v = w % u;
            lowerLeftCorner = origin - halfWidth * focusDist * u - halfHeight * focusDist * v - focusDist * w;
            horizontal = 2.0 * halfWidth * focusDist * u;
            vertical = 2.0 * halfHeight * focusDist * v;
        }
        
        ThRayf GetRay(float u, float v)
        {
            ThVec3f direction = lowerLeftCorner + u * horizontal + v * vertical - origin;
            direction.Normalize();
            return ThRayf(origin, direction);
        }
        
        ThRayf GetRayLens(float u, float v)
        {
            ThVec3f rd = RandomInUnitDisk();
            ThVec3f offset;
            offset.x() = lensRadius * u * rd.x();
            offset.y() = lensRadius * v * rd.y();
            ThVec3f direction = lowerLeftCorner + u * horizontal + v * vertical - origin - offset;
            direction.Normalize();
            return ThRayf(origin + offset, direction);
        }
        
        ThVec3f RandomInUnitDisk()
        {
            ThVec3f result;
            
            do
            {
                result.x() = UniformDistribution(-1.0, 1.0);
                result.y() = UniformDistribution(-1.0, 1.0);
                result.z() = 0.0;
            }
            while (result * result > 1.0);
                
            return result;
        }
        
        ThVec3f lowerLeftCorner;
        ThVec3f horizontal;
        ThVec3f vertical;
        ThVec3f origin;
        ThVec3f u;
        ThVec3f v;
        ThVec3f w;
        ThF32 lensRadius;
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
        m_TraceDepth(1),
        m_CameraOrigin(0.0, 0.0, 0.0),
        m_CameraLookAt(0.0, 0.0, -1.0),
        m_CameraUp(0.0, 1.0, 0.0),
        m_CameraFov(90.0),
        m_CameraMode(CameraMode::Normal),
        m_CameraAperture(2.0),
        m_CameraFocusDist(1.0)
        {
            
        }
        
        ThI32 m_Width;
        ThI32 m_Height;
        ThI32 m_FramesToRender;
        ThI32 m_SamplesPerPixel;
        ThI32 m_TraceDepth;
        CameraMode m_CameraMode;
        ThF32 m_CameraFov;
        ThF32 m_CameraAperture;
        ThF32 m_CameraFocusDist;
        ThVec3f m_CameraOrigin;
        ThVec3f m_CameraLookAt;
        ThVec3f m_CameraUp;
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
        static void RenderFunc(void* ctx);
        static void SyncFunc(void* ctx);
        
        struct WorkItem
        {
            RayTracer* m_Self;
            float m_OneOverW;
            float m_OneOverH;
            ThI32 m_J;
        };
        
        std::atomic<RayTracerState> m_State;
        Film* m_Film;        
        ThI32 m_FramesRendered;
        Scene* m_Scene;
        std::mt19937 m_Generator;
        std::uniform_real_distribution<double> m_RngUniform;
        RayTracerOptions m_Options;
        Camera m_Camera;
        ThDispatchQueue m_Queue;
        ThDispatchGroup m_Group;
        ThVector<WorkItem> m_WorkItems;
    };
}
