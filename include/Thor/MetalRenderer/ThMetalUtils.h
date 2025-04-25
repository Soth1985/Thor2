#pragma once

#include <Thor/SimdMath/Simd.h>
#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>

namespace Thor
{

struct ThFramebufferDescriptor
{
    ThU32 m_SampleCount = 1;
    MTL::PixelFormat m_DepthStencilPixelFormat = MTL::PixelFormatDepth32Float_Stencil8;
    MTL::PixelFormat m_ColorPixelFormat = MTL::PixelFormatBGRA8Unorm;
    simd::float4 m_ClearColor = {0.0, 0.0, 0.0, 0.0};
    ThF64 m_ClearDepth = 1.0;
};

class ThMetalUtils
{
public:
    static NS::SharedPtr<MTL::Device> CreateDefaultDevice();
    static MTL::ResourceOptions GetDefaultBufferOptions();
    static MTL::StorageMode GetDefaultTextureStorageMode();
};

}
