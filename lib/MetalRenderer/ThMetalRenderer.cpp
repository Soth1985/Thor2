#include <Thor/MetalRenderer/ThMetalRenderer.h>
#include <Thor/Core/Debug/ThLogger.h>

using namespace Thor;

ThMetalRenderer::ThMetalRenderer(NS::SharedPtr<MTL::Device> device)
    :
m_Device(device)
{
   
}
