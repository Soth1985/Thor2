#include "MetalRendererSample0.h"

#include <Thor/Core/Debug/ThLogger.h>

MetalRendererSample0::MetalRendererSample0(NS::SharedPtr<MTL::Device> device)
    :
MetalRenderer(device)
{
    
}

void MetalRendererSample0::SetupRendering()
{
    m_CommandQueue = NS::TransferPtr(m_Device->newCommandQueue());
}

void MetalRendererSample0::RenderFrame(MTL::Viewport viewport, MTL::RenderPassDescriptor* renderPassDescriptor, MTL::Drawable* drawable)
{
    MTL::CommandBuffer* commandBuffer = m_CommandQueue->commandBuffer();
    commandBuffer->setLabel(NS::String::string("Main Command Buffer", NS::UTF8StringEncoding));
    
    commandBuffer->addCompletedHandler( ^void( MTL::CommandBuffer* pCmd )
    {
        
    });

    MTL::RenderCommandEncoder* renderEncoder = commandBuffer->renderCommandEncoder(renderPassDescriptor);
    renderEncoder->setLabel(NS::String::string("Final Pass Encoder", NS::UTF8StringEncoding));
    renderEncoder->endEncoding();

    commandBuffer->presentDrawable(drawable);
    commandBuffer->commit();
}

void MetalRendererSample0::ViewportSizeChanged(MTL::Viewport viewport)
{
    
}
