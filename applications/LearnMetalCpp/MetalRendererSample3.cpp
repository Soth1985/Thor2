#include "MetalRendererSample3.h"

#include <Thor/Core/Debug/ThLogger.h>
#include <Thor/Math/Simd/Simd.h>

struct FrameData
{
    float angle;
};

MetalRendererSample3::MetalRendererSample3(NS::SharedPtr<MTL::Device> device)
    :
MetalRenderer(device)
{
    
}

void MetalRendererSample3::SetupRendering()
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
    
    for ( int i = 0; i < kMaxFramesInFlight; ++i )
    {
        m_PerFrameBuffers[ i ] = NS::TransferPtr(m_Device->newBuffer(sizeof(FrameData), MTL::ResourceStorageModeManaged));
    }
    
    auto vertexFn = NS::TransferPtr(m_DefaultLibrary->newFunction( NS::String::string("vertexMain3", NS::UTF8StringEncoding)));
    auto fragFn = NS::TransferPtr(m_DefaultLibrary->newFunction( NS::String::string("fragmentMain3", NS::UTF8StringEncoding)));
    
    auto argEncoder = NS::TransferPtr(vertexFn->newArgumentEncoder(0));
    m_ArgBuffer = NS::TransferPtr(m_Device->newBuffer( argEncoder->encodedLength(), MTL::ResourceStorageModeManaged));
    argEncoder->setArgumentBuffer(m_ArgBuffer.get(), 0);
    argEncoder->setBuffer(m_PositionBuffer.get(), 0, 0);
    argEncoder->setBuffer(m_ColorBuffer.get(), 0, 1);
    m_ArgBuffer->didModifyRange(NS::Range::Make(0, m_ArgBuffer->length()));

    auto pDesc = NS::TransferPtr(MTL::RenderPipelineDescriptor::alloc()->init());
    pDesc->setVertexFunction(vertexFn.get());
    pDesc->setFragmentFunction(fragFn.get());
    pDesc->colorAttachments()->object(0)->setPixelFormat( MTL::PixelFormat::PixelFormatBGRA8Unorm);
    pDesc->setDepthAttachmentPixelFormat(MTL::PixelFormat::PixelFormatDepth32Float_Stencil8);
    pDesc->setStencilAttachmentPixelFormat(MTL::PixelFormat::PixelFormatDepth32Float_Stencil8);

    NS::Error* pError = nullptr;
    m_PipelineState = NS::TransferPtr(m_Device->newRenderPipelineState(pDesc.get(), &pError));
    
    if (pError)
    {
        auto message = pError->localizedDescription()->utf8String();
        THOR_ERR("Failed to create pipeline state: %s", "Metal", message);
    }
}

void MetalRendererSample3::RenderFrame(MTL::Viewport viewport, MTL::RenderPassDescriptor* renderPassDescriptor, MTL::Drawable* drawable)
{
    MTL::CommandBuffer* commandBuffer = m_CommandQueue->commandBuffer();
    commandBuffer->setLabel(NS::String::string("Main Command Buffer", NS::UTF8StringEncoding));
    
    m_Frame = (m_Frame + 1) % kMaxFramesInFlight;
    auto perFrameDataBuffer = m_PerFrameBuffers[m_Frame];
    
    m_Semaphore.Wait();
    
    commandBuffer->addCompletedHandler( ^void( MTL::CommandBuffer* pCmd )
    {
        m_Semaphore.Signal();
    });
    
    m_Angle += 0.01f;
    reinterpret_cast<FrameData*>(perFrameDataBuffer->contents())->angle = m_Angle;
    perFrameDataBuffer->didModifyRange(NS::Range::Make(0, sizeof(FrameData)));

    MTL::RenderCommandEncoder* renderEncoder = commandBuffer->renderCommandEncoder(renderPassDescriptor);
    renderEncoder->setLabel(NS::String::string("Final Pass Encoder", NS::UTF8StringEncoding));
    renderEncoder->setViewport(viewport);
    renderEncoder->pushDebugGroup(NS::String::string("Render Triangle", NS::UTF8StringEncoding));
    renderEncoder->setRenderPipelineState(m_PipelineState.get());
    renderEncoder->setVertexBuffer(m_ArgBuffer.get(), 0, 0);
    renderEncoder->setVertexBuffer(perFrameDataBuffer.get(), 0, 1);
    renderEncoder->useResource(m_ColorBuffer.get(), MTL::ResourceUsageRead);
    renderEncoder->useResource(m_PositionBuffer.get(), MTL::ResourceUsageRead);
    renderEncoder->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(3));
    renderEncoder->popDebugGroup();
    renderEncoder->endEncoding();

    commandBuffer->presentDrawable(drawable);
    commandBuffer->commit();
}

void MetalRendererSample3::ViewportSizeChanged(MTL::Viewport viewport)
{
    
}
