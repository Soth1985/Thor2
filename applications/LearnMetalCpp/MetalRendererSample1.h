#pragma once

#include <Thor/MetalRenderer/ThMetalRenderer.h>
#include <Metal/Metal.hpp>

class MetalRendererSample1 : public Thor::ThMetalRenderer
{
public:
    ~MetalRendererSample1() = default;
    MetalRendererSample1(NS::SharedPtr<MTL::Device> device);
    virtual void SetupRendering()override;
    virtual void RenderFrame(MTL::Viewport viewport, MTL::RenderPassDescriptor* renderPassDescriptor, MTL::Drawable* drawable)override;
    virtual void ViewportSizeChanged(MTL::Viewport viewport)override;
protected:
    NS::SharedPtr<MTL::CommandQueue> m_CommandQueue;
    NS::SharedPtr<MTL::Library> m_DefaultLibrary;
    NS::SharedPtr<MTL::RenderPipelineState> m_PipelineState;
    NS::SharedPtr<MTL::Buffer> m_PositionBuffer;
    NS::SharedPtr<MTL::Buffer> m_ColorBuffer;
};
