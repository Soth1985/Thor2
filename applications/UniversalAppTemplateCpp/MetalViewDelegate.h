#pragma once

#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

class MetalViewDelegate : public MTK::ViewDelegate
{
public:
    MetalViewDelegate(NS::SharedPtr<MTL::Device> device, MTK::View* view);
    virtual ~MetalViewDelegate()override = default;
    virtual void drawInMTKView( MTK::View* pView ) override;
private:
    NS::SharedPtr<MTL::Device> m_Device;
}
