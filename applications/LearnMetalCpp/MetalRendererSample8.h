#pragma once

#include <Thor/MetalRenderer/ThMetalRenderer.h>
#include <Metal/Metal.hpp>
#include <Thor/Core/Concurrent/ThDispatch.h>

#include <chrono>
#include <time.h>

class MetalRendererSample8 : public Thor::ThMetalRenderer
{
public:
    ~MetalRendererSample8() = default;
    MetalRendererSample8(NS::SharedPtr<MTL::Device> device);
    virtual void SetupRendering()override;
    virtual void RenderFrame(MTL::Viewport viewport, MTL::RenderPassDescriptor* renderPassDescriptor, MTL::Drawable* drawable)override;
    virtual void ViewportSizeChanged(MTL::Viewport viewport)override;
protected:
    NS::SharedPtr<MTL::CommandQueue> m_CommandQueue;
    NS::SharedPtr<MTL::Library> m_DefaultLibrary;
    NS::SharedPtr<MTL::RenderPipelineState> m_PipelineState;
    NS::SharedPtr<MTL::DepthStencilState> m_DepthStencilState;
    NS::SharedPtr<MTL::ComputePipelineState> m_ComputePipelineState;
    
    NS::SharedPtr<MTL::Buffer> m_PositionBuffer;
    NS::SharedPtr<MTL::Buffer> m_IndexBuffer;
    NS::SharedPtr<MTL::Buffer> m_TextureAnimationBuffer;
    NS::SharedPtr<MTL::Texture> m_Texture;
    
    static const uint32_t kTextureWidth = 128;
    static const uint32_t kTextureHeight = 128;
    static constexpr double kAutoCaptureTimeoutSecs = std::chrono::seconds(3).count();
    static const Thor::ThI32 kMaxFramesInFlight = 3;
    static const Thor::ThI32 kInstanceRows = 10;
    static const Thor::ThI32 kInstanceColumns = 10;
    static const Thor::ThI32 kInstanceDepth = 10;
    static const Thor::ThI32 kNumInstances = (kInstanceRows * kInstanceColumns * kInstanceDepth);
    Thor::ThDispatchSemaphore m_Semaphore = {kMaxFramesInFlight};
    NS::SharedPtr<MTL::Buffer> m_InstanceBuffers[kMaxFramesInFlight];
    NS::SharedPtr<MTL::Buffer> m_CameraDataBuffer[kMaxFramesInFlight];
    
    Thor::ThF32 m_Angle = 0.0f;
    Thor::ThI32 m_Frame = 0;
    Thor::ThU32 m_AnimationIndex = 0;
    bool m_BeginCapture = false;
    bool m_HasCaptured = false;
    NS::String* m_TraceSaveFilePath;
    
    void SetupCompute();
    void GenerateTexture();
    void TriggerCapture();
};

