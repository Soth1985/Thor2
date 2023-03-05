#include <Thor/MetalRenderer/ThMetalContext.h>

using namespace Thor;

NS::SharedPtr<MTL::Device> ThMetalContext::m_DefaultDevice;
ThFramebufferDescriptor ThMetalContext::m_FramebufferDescriptor;

void ThMetalContext::Initialize(const ThFramebufferDescriptor& frameBufferDesc)
{
    m_DefaultDevice = NS::TransferPtr(MTL::CreateSystemDefaultDevice());
    m_FramebufferDescriptor = frameBufferDesc;
}

NS::SharedPtr<MTL::Device> ThMetalContext::DefaultDevice()
{
    return m_DefaultDevice;
}

const ThFramebufferDescriptor& ThMetalContext::GetFramebufferDescriptor()
{
    return m_FramebufferDescriptor;
}
