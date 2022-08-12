#include "MetalRendererTriangle.h"

#include <Thor/Core/Debug/ThLogger.h>

MetalRendererTriangle::MetalRendererTriangle(NS::SharedPtr<MTL::Device> device)
    :
MetalRenderer(device)
{
    
}

void MetalRendererTriangle::SetupRendering()
{
    m_CommandQueue = NS::TransferPtr(m_Device->newCommandQueue());
    m_DefaultLibrary = NS::TransferPtr(m_Device->newDefaultLibrary());
    
    float Vertices[] =
    {
        0.0f, 0.8f,
        1.0f, 0.0f, 0.0f, 1.0f,
        0.8f, -0.8f,
        0.0f, 1.0f, 0.0f, 1.0f,
        -0.8f, -0.8f,
        0.0f, 0.0f, 1.0f, 1.0f
    };
    
    MTL::ResourceOptions resourceOptions;
#ifdef THOR_PLATFORM_IOS
    resourceOptions = MTL::StorageMode::StorageModeShared;
#else
    resourceOptions = MTL::StorageMode::StorageModeManaged;
#endif
    
    m_VertexBuffer = NS::TransferPtr(m_Device->newBuffer(Vertices, sizeof(Vertices), resourceOptions));
    auto vertexFunc = NS::TransferPtr(m_DefaultLibrary->newFunction(NS::String::string("vertexFunc", NS::UTF8StringEncoding)));
    auto fragmentFunc = NS::TransferPtr(m_DefaultLibrary->newFunction(NS::String::string("fragmentFunc", NS::UTF8StringEncoding)));
    auto renderPipelineDescriptor = NS::TransferPtr(MTL::RenderPipelineDescriptor::alloc()->init());
    
    auto vertexDescriptor = NS::TransferPtr(MTL::VertexDescriptor::alloc()->init());
    auto attribute0 = vertexDescriptor->attributes()->object(0);
    attribute0->setFormat(MTL::VertexFormat::VertexFormatFloat2);
    attribute0->setBufferIndex(0);
    attribute0->setOffset(0);
    auto attribute1 = vertexDescriptor->attributes()->object(1);
    attribute1->setFormat(MTL::VertexFormat::VertexFormatFloat4);
    attribute1->setBufferIndex(0);
    attribute1->setOffset(2 * sizeof(float));
    auto layout = vertexDescriptor->layouts()->object(0);
    layout->setStride(6 * sizeof(float));
    layout->setStepFunction(MTL::VertexStepFunction::VertexStepFunctionPerVertex);
    
    renderPipelineDescriptor->setVertexFunction(vertexFunc.get());
    renderPipelineDescriptor->setFragmentFunction(fragmentFunc.get());
    renderPipelineDescriptor->setVertexDescriptor(vertexDescriptor.get());
    renderPipelineDescriptor->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm);
    renderPipelineDescriptor->setDepthAttachmentPixelFormat(MTL::PixelFormat::PixelFormatDepth32Float_Stencil8);
    renderPipelineDescriptor->setStencilAttachmentPixelFormat(MTL::PixelFormat::PixelFormatDepth32Float_Stencil8);
    
    NS::Error* pError = nullptr;
    m_PipelineState = NS::TransferPtr(m_Device->newRenderPipelineState(renderPipelineDescriptor.get(), &pError));
    
    if (pError)
    {
        auto message = pError->localizedDescription()->utf8String();
        THOR_ERR("Failed to create pipeline state: %s", "Metal", message);
    }
}

void MetalRendererTriangle::RenderFrame(MTL::Viewport viewport, MTL::RenderPassDescriptor* renderPassDescriptor, MTL::Drawable* drawable)
{
    NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();

    MTL::CommandBuffer* commandBuffer = m_CommandQueue->commandBuffer();
    commandBuffer->setLabel(NS::String::string("Main Command Buffer", NS::UTF8StringEncoding));
    
    commandBuffer->addCompletedHandler( ^void( MTL::CommandBuffer* pCmd )
    {
        
    });

    MTL::RenderCommandEncoder* renderEncoder = commandBuffer->renderCommandEncoder(renderPassDescriptor);
    renderEncoder->setLabel(NS::String::string("Final Pass Encoder", NS::UTF8StringEncoding));
    renderEncoder->setViewport(viewport);
    renderEncoder->pushDebugGroup(NS::String::string("Render Triangle", NS::UTF8StringEncoding));
    renderEncoder->setRenderPipelineState(m_PipelineState.get());
    renderEncoder->setVertexBuffer(m_VertexBuffer.get(), 0, 0);
    renderEncoder->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(3));
    renderEncoder->popDebugGroup();
    renderEncoder->endEncoding();

    commandBuffer->presentDrawable(drawable);
    commandBuffer->commit();

    pool->release();
}

void MetalRendererTriangle::ViewportSizeChanged(MTL::Viewport viewport)
{
    
}
