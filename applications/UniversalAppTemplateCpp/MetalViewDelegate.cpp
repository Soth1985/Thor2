#include "MetalViewDelegate.h"

MetalViewDelegate::MetalViewDelegate(NS::SharedPtr<MTL::Device> device, MTK::View* view)
:
m_Device(device)
{
    view->setDevice(m_Device.get());
    view->setDelegate(this);
}

void MetalViewDelegate::drawInMTKView(MTK::View* pView)
{
    
}
