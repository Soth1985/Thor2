#pragma once

#include <Thor/MetalRenderer/ThMetalRenderer.h>
#include <Metal/Metal.hpp>

class MetalRendererSample2 : public Thor::ThMetalRenderer
{
public:
    ~MetalRendererSample2() = default;
    MetalRendererSample2(NS::SharedPtr<MTL::Device> device);
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
};
