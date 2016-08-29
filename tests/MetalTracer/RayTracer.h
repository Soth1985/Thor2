#pragma once

#include <Thor/Math/Math.h>
#include <Thor/Core/Containers/ThVector.h>
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
    
    enum class ShapeType : ThI32
    {
        Sphere
    };
    
    enum class MaterialType : ThI32
    {
        NormalMap
    };
    
    struct Shape
    {
        ShapeType type;
        ThI32 index;
    };
    
    struct Material
    {
        MaterialType type;
        ThI32 index;
    };
    
    struct Object
    {
        Shape shape;
        Material material;
    };
    
    struct Scene
    {
        ThVector<ThSpheref> spheres;
        ThVector<Object> objects;
        
        void AddSphere(const ThSpheref& sphere);
        
        ThVec3f TraceRay(const ThRayf& ray);
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
        m_SamplesPerRay(1)
        {
            
        }
        
        ThI32 m_Width;
        ThI32 m_Height;
        ThI32 m_FramesToRender;
        ThI32 m_SamplesPerRay;
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
        std::atomic<RayTracerState> m_State;
        Film* m_Film;        
        ThI32 m_FramesRendered;
        Scene* m_Scene;
        std::mt19937 m_Generator;
        std::uniform_real_distribution<double> m_Rng;
        RayTracerOptions m_Options;
        Camera m_Camera;
    };
}