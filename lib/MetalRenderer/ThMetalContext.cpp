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

MTL::ResourceOptions ThMetalContext::GetDefaultBufferOptions()
{
#ifdef THOR_PLATFORM_OSX
    return MTL::ResourceStorageModeManaged;
#else
    return MTL::ResourceStorageModeShared;
#endif
}

MTL::StorageMode ThMetalContext::GetDefaultTextureStorageMode()
{
#ifdef THOR_PLATFORM_OSX
    return MTL::StorageModeManaged;
#else
    return MTL::StorageModeShared;
#endif
}
