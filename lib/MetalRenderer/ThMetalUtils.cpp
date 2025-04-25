#include <Thor/MetalRenderer/ThMetalUtils.h>

using namespace Thor;

NS::SharedPtr<MTL::Device> ThMetalUtils::CreateDefaultDevice()
{
    return NS::TransferPtr(MTL::CreateSystemDefaultDevice());;
}

MTL::ResourceOptions ThMetalUtils::GetDefaultBufferOptions()
{
#ifdef THOR_PLATFORM_OSX
    return MTL::ResourceStorageModeManaged;
#else
    return MTL::ResourceStorageModeShared;
#endif
}

MTL::StorageMode ThMetalUtils::GetDefaultTextureStorageMode()
{
#ifdef THOR_PLATFORM_OSX
    return MTL::StorageModeManaged;
#else
    return MTL::StorageModeShared;
#endif
}
