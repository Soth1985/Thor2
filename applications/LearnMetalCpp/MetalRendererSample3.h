#pragma once

#include "MetalRenderer.h"
#include <Metal/Metal.hpp>
#include <Thor/Core/Concurrent/ThDispatch.h>

class MetalRendererSample3 : public MetalRenderer
{
public:
    ~MetalRendererSample3() = default;
    MetalRendererSample3(NS::SharedPtr<MTL::Device> device);
    virtual void SetupRendering()override;
    virtual void RenderFrame(MTL::Viewport viewport, MTL::RenderPassDescriptor* renderPassDescriptor, MTL::Drawable* drawable)override;
    virtual void ViewportSizeChanged(MTL::Viewport viewport)override;
protected:
    NS::SharedPtr<MTL::CommandQueue> m_CommandQueue;
    NS::SharedPtr<MTL::Library> m_DefaultLibrary;
    NS::SharedPtr<MTL::RenderPipelineState> m_PipelineState;
    NS::SharedPtr<MTL::Buffer> m_PositionBuffer;
    NS::SharedPtr<MTL::Buffer> m_ColorBuffer;
    NS::SharedPtr<MTL::Buffer> m_ArgBuffer;
    
    static const int kMaxFramesInFlight = 3;
    Thor::ThDispatchSemaphore m_Semaphore = {kMaxFramesInFlight};
    NS::SharedPtr<MTL::Buffer> m_PerFrameBuffers[kMaxFramesInFlight];
    
    float m_Angle = 0.0f;
    int m_Frame = 0;
};
