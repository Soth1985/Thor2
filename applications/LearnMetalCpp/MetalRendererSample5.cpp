#include "MetalRendererSample5.h"
#include "Shared5.h"
#include "MetalMath.h"

#include <Thor/Core/Debug/ThLogger.h>
#include <Thor/Math/Simd/Simd.h>

using namespace Thor;

MetalRendererSample5::MetalRendererSample5(NS::SharedPtr<MTL::Device> device)
    :
MetalRenderer(device)
{
    
}

void MetalRendererSample5::SetupRendering()
{
    m_CommandQueue = NS::TransferPtr(m_Device->newCommandQueue());
    m_DefaultLibrary = NS::TransferPtr(m_Device->newDefaultLibrary());
    
    const float s = 0.5f;

    float3 verts[] =
    {
        { -s, -s, +s },
        { +s, -s, +s },
        { +s, +s, +s },
        { -s, +s, +s },

        { -s, -s, -s },
        { -s, +s, -s },
        { +s, +s, -s },
        { +s, -s, -s }
    };

    uint16_t indices[] =
    {
        0, 1, 2, /* front */
        2, 3, 0,

        1, 7, 6, /* right */
        6, 2, 1,

        7, 4, 5, /* back */
        5, 6, 7,

        4, 0, 3, /* left */
        3, 5, 4,

        3, 2, 6, /* top */
        6, 5, 3,

        4, 7, 1, /* bottom */
        1, 0, 4
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

    auto vertexFn = NS::TransferPtr(m_DefaultLibrary->newFunction( NS::String::string("vertexMain5", NS::UTF8StringEncoding)));
    auto fragFn = NS::TransferPtr(m_DefaultLibrary->newFunction( NS::String::string("fragmentMain5", NS::UTF8StringEncoding)));
    
    auto argEncoder = NS::TransferPtr(vertexFn->newArgumentEncoder(0));
    m_ArgBuffer = NS::TransferPtr(m_Device->newBuffer( argEncoder->encodedLength(), MTL::ResourceStorageModeManaged));
    argEncoder->setArgumentBuffer(m_ArgBuffer.get(), 0);
    argEncoder->setBuffer(m_PositionBuffer.get(), 0, 0);
    m_ArgBuffer->didModifyRange(NS::Range::Make(0, m_ArgBuffer->length()));
    
    auto pDsDesc = NS::TransferPtr(MTL::DepthStencilDescriptor::alloc()->init());
    pDsDesc->setDepthCompareFunction(MTL::CompareFunction::CompareFunctionLess);
    pDsDesc->setDepthWriteEnabled(true);
    m_DepthStencilState = NS::TransferPtr(m_Device->newDepthStencilState(pDsDesc.get()));

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

void MetalRendererSample5::RenderFrame(MTL::Viewport viewport, MTL::RenderPassDescriptor* renderPassDescriptor, MTL::Drawable* drawable)
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
    
    m_Angle += 0.01f;
    
    const ThF32 scl = 0.2f;
    InstanceData* pInstanceData = reinterpret_cast<InstanceData*>( perInstanceDataBuffer->contents());

    float3 objectPosition = { 0.f, 0.f, -5.f };

    // Update instance positions:

    float4x4 rt = math::makeTranslate(objectPosition);
    float4x4 rr = math::makeYRotate(-m_Angle);
    float4x4 rtInv = math::makeTranslate({ -objectPosition.x, -objectPosition.y, -objectPosition.z } );
    float4x4 fullObjectRot = rt * rr * rtInv;

    for ( size_t i = 0; i < kNumInstances; ++i )
    {
        float iDivNumInstances = i / (float)kNumInstances;
        float xoff = (iDivNumInstances * 2.0f - 1.0f) + (1.f / kNumInstances);
        float yoff = sin((iDivNumInstances + m_Angle) * 2.0f * M_PI);

        // Use the tiny math library to apply a 3D transformation to the instance.
        float4x4 scale = math::makeScale((float3){scl, scl, scl });
        float4x4 zrot = math::makeZRotate(m_Angle);
        float4x4 yrot = math::makeYRotate(m_Angle);
        float4x4 translate = math::makeTranslate( math::add( objectPosition, { xoff, yoff, 0.f } ) );

        pInstanceData[ i ].instanceTransform = fullObjectRot * translate * yrot * zrot * scale;

        float r = iDivNumInstances;
        float g = 1.0f - r;
        float b = sinf( M_PI * 2.0f * iDivNumInstances );
        pInstanceData[i].instanceColor = (float4){ r, g, b, 1.0f };
    }
    
    perInstanceDataBuffer->didModifyRange(NS::Range::Make(0, perInstanceDataBuffer->length()));

    // Update camera state:

    CameraData* pCameraData = reinterpret_cast<CameraData*>( pCameraDataBuffer->contents());
    
    pCameraData->perspectiveTransform = math::makePerspective( 45.f * M_PI / 180.f, 1.f, 0.03f, 500.0f ) ;
    pCameraData->worldTransform = math::makeIdentity();
    pCameraDataBuffer->didModifyRange(NS::Range::Make(0, sizeof(CameraData)));

    MTL::RenderCommandEncoder* renderEncoder = commandBuffer->renderCommandEncoder(renderPassDescriptor);
    renderEncoder->setLabel(NS::String::string("Final Pass Encoder", NS::UTF8StringEncoding));
    renderEncoder->setViewport(viewport);
    renderEncoder->pushDebugGroup(NS::String::string("Render Triangle", NS::UTF8StringEncoding));
    renderEncoder->setRenderPipelineState(m_PipelineState.get());
    renderEncoder->setDepthStencilState(m_DepthStencilState.get());
    renderEncoder->setCullMode(MTL::CullModeBack);
    renderEncoder->setFrontFacingWinding(MTL::Winding::WindingCounterClockwise);
    renderEncoder->setVertexBuffer(m_ArgBuffer.get(), 0, 0);
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

void MetalRendererSample5::ViewportSizeChanged(MTL::Viewport viewport)
{
    
}

