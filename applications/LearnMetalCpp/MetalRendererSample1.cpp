#include "MetalRendererSample1.h"

#include <Thor/Core/Debug/ThLogger.h>
#include <Thor/MetalRenderer/ThMetalContext.h>
#include <Thor/SimdMath/Simd.h>

using namespace Thor;

MetalRendererSample1::MetalRendererSample1(NS::SharedPtr<MTL::Device> device)
    :
ThMetalRenderer(device)
{
    
}

void MetalRendererSample1::SetupRendering()
{
    m_CommandQueue = NS::TransferPtr(m_Device->newCommandQueue());
    m_DefaultLibrary = NS::TransferPtr(m_Device->newDefaultLibrary());
    
    const size_t numVertices = 3;

    simd::float3 positions[numVertices] =
    {
        { -0.8f,  0.8f, 0.0f },
        {  0.0f, -0.8f, 0.0f },
        { +0.8f,  0.8f, 0.0f }
    };

    simd::float3 colors[numVertices] =
    {
        {  1.0, 0.3f, 0.2f },
        {  0.8f, 1.0, 0.0f },
        {  0.8f, 0.0f, 1.0 }
    };

    const size_t positionsDataSize = numVertices * sizeof( simd::float3 );
    const size_t colorDataSize = numVertices * sizeof( simd::float3 );

    auto pVertexPositionsBuffer = m_Device->newBuffer(positionsDataSize, MTL::ResourceStorageModeManaged);
    auto pVertexColorsBuffer = m_Device->newBuffer(colorDataSize, MTL::ResourceStorageModeManaged);

    m_PositionBuffer = NS::TransferPtr(pVertexPositionsBuffer);
    m_ColorBuffer = NS::TransferPtr(pVertexColorsBuffer);

    memcpy(m_PositionBuffer->contents(), positions, positionsDataSize);
    memcpy(m_ColorBuffer->contents(), colors, colorDataSize );

    m_PositionBuffer->didModifyRange(NS::Range::Make(0, m_PositionBuffer->length()));
    m_ColorBuffer->didModifyRange(NS::Range::Make(0, m_ColorBuffer->length()));
    
    auto vertexFn = NS::TransferPtr(m_DefaultLibrary->newFunction( NS::String::string("vertexMain1", NS::UTF8StringEncoding)));
    auto fragFn = NS::TransferPtr(m_DefaultLibrary->newFunction( NS::String::string("fragmentMain1", NS::UTF8StringEncoding)));

    auto pDesc = NS::TransferPtr(MTL::RenderPipelineDescriptor::alloc()->init());
    pDesc->setVertexFunction(vertexFn.get());
    pDesc->setFragmentFunction(fragFn.get());
    
    auto frameBufferDesc = ThMetalContext::GetFramebufferDescriptor();
    pDesc->colorAttachments()->object(0)->setPixelFormat(frameBufferDesc.m_ColorPixelFormat);
    pDesc->setDepthAttachmentPixelFormat(frameBufferDesc.m_DepthStencilPixelFormat);
    pDesc->setStencilAttachmentPixelFormat(frameBufferDesc.m_DepthStencilPixelFormat);

    NS::Error* pError = nullptr;
    m_PipelineState = NS::TransferPtr(m_Device->newRenderPipelineState(pDesc.get(), &pError));
    
    if (pError)
    {
        auto message = pError->localizedDescription()->utf8String();
        THOR_ERR("Failed to create pipeline state: %s", "Metal", message);
    }
}

void MetalRendererSample1::RenderFrame(MTL::Viewport viewport, MTL::RenderPassDescriptor* renderPassDescriptor, MTL::Drawable* drawable)
{
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
    renderEncoder->setVertexBuffer(m_PositionBuffer.get(), 0, 0);
    renderEncoder->setVertexBuffer(m_ColorBuffer.get(), 0, 1);
    renderEncoder->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(3));
    renderEncoder->popDebugGroup();
    renderEncoder->endEncoding();

    commandBuffer->presentDrawable(drawable);
    commandBuffer->commit();
}

void MetalRendererSample1::ViewportSizeChanged(MTL::Viewport viewport)
{
    
}