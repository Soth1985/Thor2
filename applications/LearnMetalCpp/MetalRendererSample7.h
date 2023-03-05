#pragma once

#include <Thor/MetalRenderer/ThMetalRenderer.h>
#include <Metal/Metal.hpp>
#include <Thor/Core/Concurrent/ThDispatch.h>

class MetalRendererSample7 : public Thor::ThMetalRenderer
{
public:
    ~MetalRendererSample7() = default;
    MetalRendererSample7(NS::SharedPtr<MTL::Device> device);
    virtual void SetupRendering()override;
    virtual void RenderFrame(MTL::Viewport viewport, MTL::RenderPassDescriptor* renderPassDescriptor, MTL::Drawable* drawable)override;
    virtual void ViewportSizeChanged(MTL::Viewport viewport)override;
protected:
    NS::SharedPtr<MTL::CommandQueue> m_CommandQueue;
    NS::SharedPtr<MTL::Library> m_DefaultLibrary;
    NS::SharedPtr<MTL::RenderPipelineState> m_PipelineState;
    NS::SharedPtr<MTL::Buffer> m_PositionBuffer;
    NS::SharedPtr<MTL::Buffer> m_IndexBuffer;
    NS::SharedPtr<MTL::DepthStencilState> m_DepthStencilState;
    NS::SharedPtr<MTL::Texture> m_Texture;
            
    static const Thor::ThI32 kMaxFramesInFlight = 3;
    static const Thor::ThI32 kInstanceRows = 10;
    static const Thor::ThI32 kInstanceColumns = 10;
    static const Thor::ThI32 kInstanceDepth = 10;
    static const Thor::ThI32 kNumInstances = (kInstanceRows * kInstanceColumns * kInstanceDepth);
    Thor::ThDispatchSemaphore m_Semaphore = {kMaxFramesInFlight};
    NS::SharedPtr<MTL::Buffer> m_InstanceBuffers[kMaxFramesInFlight];
    NS::SharedPtr<MTL::Buffer> m_CameraDataBuffer[kMaxFramesInFlight];
    
    Thor::ThF32 m_Angle = 0.0f;
    Thor::ThI32 m_Frame = 0;
};

