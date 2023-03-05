#include "MetalRendererSample4.h"
#include "Shared4.h"

#include <Thor/Core/Debug/ThLogger.h>
#include <Thor/SimdMath/Simd.h>

using namespace Thor;

MetalRendererSample4::MetalRendererSample4(NS::SharedPtr<MTL::Device> device)
    :
MetalRenderer(device)
{
    
}

void MetalRendererSample4::SetupRendering()
{
    m_CommandQueue = NS::TransferPtr(m_Device->newCommandQueue());
    m_DefaultLibrary = NS::TransferPtr(m_Device->newDefaultLibrary());
    
    const float s = 0.5f;

    float3 verts[] =
    {
        { -s, -s, +s },
        { +s, -s, +s },
        { +s, +s, +s },
        { -s, +s, +s }
    };

    uint16_t indices[] =
    {
        0, 1, 2,
        2, 3, 0,
    };

    const size_t vertexDataSize = sizeof( verts );
    const size_t indexDataSize = sizeof( indices );

    auto pVertexPositionsBuffer = m_Device->newBuffer(vertexDataSize, MTL::ResourceStorageModeManaged);
    auto pIndexBuffer = m_Device->newBuffer(indexDataSize, MTL::ResourceStorageModeManaged);

    m_PositionBuffer = NS::TransferPtr(pVertexPositionsBuffer);
    m_IndexBuffer = NS::TransferPtr(pIndexBuffer);

    memcpy(m_PositionBuffer->contents(), verts, vertexDataSize);
    memcpy(m_IndexBuffer->contents(), indices, indexDataSize );

    m_PositionBuffer->didModifyRange(NS::Range::Make(0, m_PositionBuffer->length()));
    m_IndexBuffer->didModifyRange(NS::Range::Make(0, m_IndexBuffer->length()));
    
    const size_t instanceDataSize = kMaxFramesInFlight * kNumInstances * sizeof(InstanceData);
    
    for ( size_t i = 0; i < kMaxFramesInFlight; ++i )
    {
        m_InstanceBuffers[i] = NS::TransferPtr(m_Device->newBuffer(instanceDataSize, MTL::ResourceStorageModeManaged));
    }

    auto vertexFn = NS::TransferPtr(m_DefaultLibrary->newFunction( NS::String::string("vertexMain4", NS::UTF8StringEncoding)));
    auto fragFn = NS::TransferPtr(m_DefaultLibrary->newFunction( NS::String::string("fragmentMain4", NS::UTF8StringEncoding)));
    
    auto argEncoder = NS::TransferPtr(vertexFn->newArgumentEncoder(0));
    m_ArgBuffer = NS::TransferPtr(m_Device->newBuffer( argEncoder->encodedLength(), MTL::ResourceStorageModeManaged));
    argEncoder->setArgumentBuffer(m_ArgBuffer.get(), 0);
    argEncoder->setBuffer(m_PositionBuffer.get(), 0, 0);
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

void MetalRendererSample4::RenderFrame(MTL::Viewport viewport, MTL::RenderPassDescriptor* renderPassDescriptor, MTL::Drawable* drawable)
{
    MTL::CommandBuffer* commandBuffer = m_CommandQueue->commandBuffer();
    commandBuffer->setLabel(NS::String::string("Main Command Buffer", NS::UTF8StringEncoding));
    
    m_Frame = (m_Frame + 1) % kMaxFramesInFlight;
    auto perInstanceDataBuffer = m_InstanceBuffers[m_Frame];
    
    m_Semaphore.Wait();
    
    commandBuffer->addCompletedHandler( ^void( MTL::CommandBuffer* pCmd )
    {
        m_Semaphore.Signal();
    });
    
    m_Angle += 0.01f;
    
    const ThF32 scl = 0.1f;
    InstanceData* pInstanceData = reinterpret_cast<InstanceData*>( perInstanceDataBuffer->contents());
    for (ThSize i = 0; i < kNumInstances; ++i)
    {
        ThF32 iDivNumInstances = i / (ThF32)kNumInstances;
        ThF32 xoff = (iDivNumInstances * 2.0f - 1.0f) + (1.f / kNumInstances);
        ThF32 yoff = sin( ( iDivNumInstances + m_Angle ) * 2.0f * M_PI);
        pInstanceData[i].instanceTransform = (float4x4)
        {
            (float4){ scl * sinf(m_Angle), scl * cosf(m_Angle), 0.f, 0.f },
            (float4){ scl * cosf(m_Angle), scl * -sinf(m_Angle), 0.f, 0.f },
            (float4){ 0.f, 0.f, scl, 0.f },
            (float4){ xoff, yoff, 0.f, 1.f }
        };

        ThF32 r = iDivNumInstances;
        ThF32 g = 1.0f - r;
        ThF32 b = sinf( M_PI * 2.0f * iDivNumInstances );
        pInstanceData[i].instanceColor = (float4){ r, g, b, 1.0f };
    }
    
    perInstanceDataBuffer->didModifyRange(NS::Range::Make(0, perInstanceDataBuffer->length()));

    MTL::RenderCommandEncoder* renderEncoder = commandBuffer->renderCommandEncoder(renderPassDescriptor);
    renderEncoder->setLabel(NS::String::string("Final Pass Encoder", NS::UTF8StringEncoding));
    renderEncoder->setViewport(viewport);
    renderEncoder->pushDebugGroup(NS::String::string("Render Triangle", NS::UTF8StringEncoding));
    renderEncoder->setRenderPipelineState(m_PipelineState.get());
    renderEncoder->setVertexBuffer(m_ArgBuffer.get(), 0, 0);
    renderEncoder->setVertexBuffer(perInstanceDataBuffer.get(), 0, 1);
    renderEncoder->useResource(m_PositionBuffer.get(), MTL::ResourceUsageRead);
    renderEncoder->drawIndexedPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle,
                                    6, MTL::IndexType::IndexTypeUInt16,
                                    m_IndexBuffer.get(),
                                    0,
                                    kNumInstances);
    renderEncoder->popDebugGroup();
    renderEncoder->endEncoding();

    commandBuffer->presentDrawable(drawable);
    commandBuffer->commit();
}

void MetalRendererSample4::ViewportSizeChanged(MTL::Viewport viewport)
{
    
}

