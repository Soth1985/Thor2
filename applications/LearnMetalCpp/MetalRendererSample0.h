#pragma once

#include <Thor/MetalRenderer/ThMetalRenderer.h>
#include <Metal/Metal.hpp>

class MetalRendererSample0 : public Thor::ThMetalRenderer
{
public:
    ~MetalRendererSample0() = default;
    MetalRendererSample0(NS::SharedPtr<MTL::Device> device);
    virtual void SetupRendering()override;
    virtual void RenderFrame(MTL::Viewport viewport, MTL::RenderPassDescriptor* renderPassDescriptor, MTL::Drawable* drawable)override;
    virtual void ViewportSizeChanged(MTL::Viewport viewport)override;
protected:
    NS::SharedPtr<MTL::CommandQueue> m_CommandQueue;
};
