#pragma once

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>

namespace Thor
{

class ThMetalRenderer
{
public:
    virtual ~ThMetalRenderer() = default;
    ThMetalRenderer(NS::SharedPtr<MTL::Device> device);
    virtual void SetupRendering() = 0;
    virtual void RenderFrame(MTL::Viewport viewport, MTL::RenderPassDescriptor* renderPassDescriptor, MTL::Drawable* drawable) = 0;
    virtual void ViewportSizeChanged(MTL::Viewport viewport) = 0;
protected:
    NS::SharedPtr<MTL::Device> m_Device;
};

}
