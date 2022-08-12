#include "MetalRenderer.h"

#include <Thor/Core/Debug/ThLogger.h>

MetalRenderer::MetalRenderer(NS::SharedPtr<MTL::Device> device)
    :
m_Device(device)
{
   
}
