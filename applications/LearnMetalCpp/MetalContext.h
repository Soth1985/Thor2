#pragma once

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>

class MetalContext
{
public:
    static void Initialize();
    static NS::SharedPtr<MTL::Device> DefaultDevice();
private:
    static NS::SharedPtr<MTL::Device> m_DefaultDevice;
};
