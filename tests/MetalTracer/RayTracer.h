#pragma once

#include <Thor/Math/Math.h>
#include <atomic>

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
        
    };
    
    enum class MaterialType : ThI32
    {
        
    };
    
    /*struct Shape
    {
        type;
        index;
    };
    
    struct Material
    {
        type;
        index;
    };
    
    struct Object
    {
        shape;
        material;
    };*/
    
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
        m_FramesToRender(1)
        {
            
        }
        
        ThI32 m_Width;
        ThI32 m_Height;
        ThI32 m_FramesToRender;
    };
    
    class RayTracer
    {
    public:
        RayTracer();
        void Init(const RayTracerOptions& options);
        RayTracerState GetState();
        bool RenderFrame();
        const Film* GetFilm()const;
        const RayTracerOptions& GetOptions()const;
        void ResizeFilm(ThI32 width, ThI32 height);
        void FrameFetched();
    private:
        std::atomic<RayTracerState> m_State;
        Film* m_Film;
        RayTracerOptions m_Options;
        ThI32 m_FramesRendered;
    };
}