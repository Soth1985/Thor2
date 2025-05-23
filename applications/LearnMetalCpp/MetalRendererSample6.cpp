#include "MetalRendererSample6.h"
#include "Shared6.h"

#include <Thor/SimdMath/ThGraphics.h>
#include <Thor/Core/Debug/ThLogger.h>
#include <Thor/MetalRenderer/ThMetalUtils.h>
#include <Thor/SimdMath/Simd.h>

using namespace Thor;

MetalRendererSample6::MetalRendererSample6(NS::SharedPtr<MTL::Device> device)
    :
ThMetalRenderer(device)
{
    
}

void MetalRendererSample6::SetupRendering()
{
    m_CommandQueue = NS::TransferPtr(m_Device->newCommandQueue());
    m_DefaultLibrary = NS::TransferPtr(m_Device->newDefaultLibrary());
    
    const float s = 0.5f;

    VertexData verts[] =
    {
        //   Positions          Normals
        { { -s, -s, +s }, { 0.f,  0.f,  1.f } },
        { { +s, -s, +s }, { 0.f,  0.f,  1.f } },
        { { +s, +s, +s }, { 0.f,  0.f,  1.f } },
        { { -s, +s, +s }, { 0.f,  0.f,  1.f } },

        { { +s, -s, +s }, { 1.f,  0.f,  0.f } },
        { { +s, -s, -s }, { 1.f,  0.f,  0.f } },
        { { +s, +s, -s }, { 1.f,  0.f,  0.f } },
        { { +s, +s, +s }, { 1.f,  0.f,  0.f } },

        { { +s, -s, -s }, { 0.f,  0.f, -1.f } },
        { { -s, -s, -s }, { 0.f,  0.f, -1.f } },
        { { -s, +s, -s }, { 0.f,  0.f, -1.f } },
        { { +s, +s, -s }, { 0.f,  0.f, -1.f } },

        { { -s, -s, -s }, { -1.f, 0.f,  0.f } },
        { { -s, -s, +s }, { -1.f, 0.f,  0.f } },
        { { -s, +s, +s }, { -1.f, 0.f,  0.f } },
        { { -s, +s, -s }, { -1.f, 0.f,  0.f } },

        { { -s, +s, +s }, { 0.f,  1.f,  0.f } },
        { { +s, +s, +s }, { 0.f,  1.f,  0.f } },
        { { +s, +s, -s }, { 0.f,  1.f,  0.f } },
        { { -s, +s, -s }, { 0.f,  1.f,  0.f } },

        { { -s, -s, -s }, { 0.f, -1.f,  0.f } },
        { { +s, -s, -s }, { 0.f, -1.f,  0.f } },
        { { +s, -s, +s }, { 0.f, -1.f,  0.f } },
        { { -s, -s, +s }, { 0.f, -1.f,  0.f } },
    };

    uint16_t indices[] =
    {
         0,  1,  2,  2,  3,  0, /* front */
         4,  5,  6,  6,  7,  4, /* right */
         8,  9, 10, 10, 11,  8, /* back */
        12, 13, 14, 14, 15, 12, /* left */
        16, 17, 18, 18, 19, 16, /* top */
        20, 21, 22, 22, 23, 20, /* bottom */
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
    
    const size_t cameraDataSize = kMaxFramesInFlight * sizeof(CameraData);
    
    for ( size_t i = 0; i < kMaxFramesInFlight; ++i )
    {
        m_CameraDataBuffer[i] = NS::TransferPtr(m_Device->newBuffer(cameraDataSize, MTL::ResourceStorageModeManaged));
    }

    auto vertexFn = NS::TransferPtr(m_DefaultLibrary->newFunction( NS::String::string("vertexMain6", NS::UTF8StringEncoding)));
    auto fragFn = NS::TransferPtr(m_DefaultLibrary->newFunction( NS::String::string("fragmentMain6", NS::UTF8StringEncoding)));
    
    auto pDsDesc = NS::TransferPtr(MTL::DepthStencilDescriptor::alloc()->init());
    pDsDesc->setDepthCompareFunction(MTL::CompareFunction::CompareFunctionLess);
    pDsDesc->setDepthWriteEnabled(true);
    m_DepthStencilState = NS::TransferPtr(m_Device->newDepthStencilState(pDsDesc.get()));

    auto pDesc = NS::TransferPtr(MTL::RenderPipelineDescriptor::alloc()->init());
    pDesc->setVertexFunction(vertexFn.get());
    pDesc->setFragmentFunction(fragFn.get());
    
    auto frameBufferDesc = ThFramebufferDescriptor{};
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

void MetalRendererSample6::RenderFrame(MTL::Viewport viewport, MTL::RenderPassDescriptor* renderPassDescriptor, MTL::Drawable* drawable)
{
    MTL::CommandBuffer* commandBuffer = m_CommandQueue->commandBuffer();
    commandBuffer->setLabel(NS::String::string("Main Command Buffer", NS::UTF8StringEncoding));
    
    m_Frame = (m_Frame + 1) % kMaxFramesInFlight;
    
    auto perInstanceDataBuffer = m_InstanceBuffers[m_Frame];
    auto pCameraDataBuffer = m_CameraDataBuffer[m_Frame];
    
    m_Semaphore.Wait();
    
    commandBuffer->addCompletedHandler( ^void( MTL::CommandBuffer* pCmd )
    {
        m_Semaphore.Signal();
    });
    
    const float scl = 0.2f;
    InstanceData* pInstanceData = reinterpret_cast<InstanceData*>( perInstanceDataBuffer->contents());

    float3 objectPosition = { 0.f, 0.f, -10.f };

    float4x4 rt = ThGraphics::MakeTranslate(objectPosition);
    float4x4 rr1 = ThGraphics::MakeYRotate(-m_Angle);
    float4x4 rr0 = ThGraphics::MakeXRotate(m_Angle * 0.5);
    float4x4 rtInv = ThGraphics::MakeTranslate({ -objectPosition.x, -objectPosition.y, -objectPosition.z });
    float4x4 fullObjectRot = rt * rr1 * rr0 * rtInv;

    size_t ix = 0;
    size_t iy = 0;
    size_t iz = 0;
    for ( size_t i = 0; i < kNumInstances; ++i )
    {
        if ( ix == kInstanceRows )
        {
            ix = 0;
            iy += 1;
        }
        
        if ( iy == kInstanceRows )
        {
            iy = 0;
            iz += 1;
        }

        float4x4 scale = ThGraphics::MakeScale((float3){ scl, scl, scl });
        float4x4 zrot = ThGraphics::MakeZRotate(m_Angle * sinf((float)ix));
        float4x4 yrot = ThGraphics::MakeYRotate(m_Angle * cosf((float)iy));

        float x = ((float)ix - (float)kInstanceRows/2.f) * (2.f * scl) + scl;
        float y = ((float)iy - (float)kInstanceColumns/2.f) * (2.f * scl) + scl;
        float z = ((float)iz - (float)kInstanceDepth/2.f) * (2.f * scl);
        float4x4 translate = ThGraphics::MakeTranslate(objectPosition + simd::float3{ x, y, z });

        pInstanceData[ i ].instanceTransform = fullObjectRot * translate * yrot * zrot * scale;
        pInstanceData[ i ].instanceNormalTransform = ThGraphics::DiscardTranslation(pInstanceData[ i ].instanceTransform);

        float iDivNumInstances = i / (float)kNumInstances;
        float r = iDivNumInstances;
        float g = 1.0f - r;
        float b = sinf( M_PI * 2.0f * iDivNumInstances );
        pInstanceData[ i ].instanceColor = (float4){ r, g, b, 1.0f };

        ix += 1;
    }
    
    perInstanceDataBuffer->didModifyRange(NS::Range::Make(0, perInstanceDataBuffer->length()));

    // Update camera state:

    CameraData* pCameraData = reinterpret_cast<CameraData*>( pCameraDataBuffer->contents());
    
    pCameraData->perspectiveTransform = ThGraphics::MakePerspective(45.f * M_PI / 180.f, 1.f, 0.03f, 500.0f) ;
    pCameraData->worldTransform = ThGraphics::MakeIdentity();
    pCameraData->worldNormalTransform = ThGraphics::DiscardTranslation(pCameraData->worldTransform);
    pCameraDataBuffer->didModifyRange(NS::Range::Make(0, sizeof(CameraData)));

    MTL::RenderCommandEncoder* renderEncoder = commandBuffer->renderCommandEncoder(renderPassDescriptor);
    renderEncoder->setLabel(NS::String::string("Final Pass Encoder", NS::UTF8StringEncoding));
    renderEncoder->setViewport(viewport);
    renderEncoder->pushDebugGroup(NS::String::string("Render Triangle", NS::UTF8StringEncoding));
    renderEncoder->setRenderPipelineState(m_PipelineState.get());
    renderEncoder->setDepthStencilState(m_DepthStencilState.get());
    renderEncoder->setCullMode(MTL::CullModeBack);
    renderEncoder->setFrontFacingWinding(MTL::Winding::WindingCounterClockwise);
    renderEncoder->setVertexBuffer(m_PositionBuffer.get(), 0, 0);
    renderEncoder->setVertexBuffer(perInstanceDataBuffer.get(), 0, 1);
    renderEncoder->setVertexBuffer(pCameraDataBuffer.get(), 0, 2);
    renderEncoder->useResource(m_PositionBuffer.get(), MTL::ResourceUsageRead);
    renderEncoder->drawIndexedPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle,
                                    36, MTL::IndexType::IndexTypeUInt16,
                                    m_IndexBuffer.get(),
                                    0,
                                    kNumInstances);
    renderEncoder->popDebugGroup();
    renderEncoder->endEncoding();

    commandBuffer->presentDrawable(drawable);
    commandBuffer->commit();
}

void MetalRendererSample6::ViewportSizeChanged(MTL::Viewport viewport)
{
    
}

