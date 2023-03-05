#pragma once

#include <Thor/MetalRenderer/ThMetalRenderer.h>
#include <Metal/Metal.hpp>
#include <Thor/Core/Concurrent/ThDispatch.h>

class MetalRendererSample4 : public Thor::ThMetalRenderer
{
public:
    ~MetalRendererSample4() = default;
    MetalRendererSample4(NS::SharedPtr<MTL::Device> device);
    virtual void SetupRendering()override;
    virtual void RenderFrame(MTL::Viewport viewport, MTL::RenderPassDescriptor* renderPassDescriptor, MTL::Drawable* drawable)override;
    virtual void ViewportSizeChanged(MTL::Viewport viewport)override;
protected:
    NS::SharedPtr<MTL::CommandQueue> m_CommandQueue;
    NS::SharedPtr<MTL::Library> m_DefaultLibrary;
    NS::SharedPtr<MTL::RenderPipelineState> m_PipelineState;
    NS::SharedPtr<MTL::Buffer> m_PositionBuffer;
    NS::SharedPtr<MTL::Buffer> m_IndexBuffer;
    NS::SharedPtr<MTL::Buffer> m_ArgBuffer;
    
    static const Thor::ThI32 kMaxFramesInFlight = 3;
    static const Thor::ThI32 kNumInstances = 16;
    Thor::ThDispatchSemaphore m_Semaphore = {kMaxFramesInFlight};
    NS::SharedPtr<MTL::Buffer> m_InstanceBuffers[kMaxFramesInFlight];
    
    Thor::ThF32 m_Angle = 0.0f;
    Thor::ThI32 m_Frame = 0;
};

