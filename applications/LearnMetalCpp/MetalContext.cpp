#include "MetalContext.h"

NS::SharedPtr<MTL::Device> MetalContext::m_DefaultDevice;

void MetalContext::Initialize()
{
    m_DefaultDevice = NS::TransferPtr(MTL::CreateSystemDefaultDevice());
}

NS::SharedPtr<MTL::Device> MetalContext::DefaultDevice()
{
    return m_DefaultDevice;
}
