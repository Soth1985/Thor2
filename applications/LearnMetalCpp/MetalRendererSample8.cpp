#include "MetalRendererSample8.h"
#include "Shared8.h"
#include "MetalMath.h"

#include <Thor/Core/Debug/ThLogger.h>
#include <Thor/Math/Simd/Simd.h>
//#include <AppKit/AppKit.hpp>

using namespace Thor;

extern "C" NS::String* NSTemporaryDirectory( void );
auto start = std::chrono::system_clock::now();

MetalRendererSample8::MetalRendererSample8(NS::SharedPtr<MTL::Device> device)
    :
MetalRenderer(device)
{
    
}

void MetalRendererSample8::SetupRendering()
{
    m_CommandQueue = NS::TransferPtr(m_Device->newCommandQueue());
    m_DefaultLibrary = NS::TransferPtr(m_Device->newDefaultLibrary());
    
    const float s = 0.5f;

    VertexData verts[] =
    {
        { { -s, -s, +s }, {  0.f,  0.f,  1.f }, { 0.f, 1.f } },
        { { +s, -s, +s }, {  0.f,  0.f,  1.f }, { 1.f, 1.f } },
        { { +s, +s, +s }, {  0.f,  0.f,  1.f }, { 1.f, 0.f } },
        { { -s, +s, +s }, {  0.f,  0.f,  1.f }, { 0.f, 0.f } },

        { { +s, -s, +s }, {  1.f,  0.f,  0.f }, { 0.f, 1.f } },
        { { +s, -s, -s }, {  1.f,  0.f,  0.f }, { 1.f, 1.f } },
        { { +s, +s, -s }, {  1.f,  0.f,  0.f }, { 1.f, 0.f } },
        { { +s, +s, +s }, {  1.f,  0.f,  0.f }, { 0.f, 0.f } },

        { { +s, -s, -s }, {  0.f,  0.f, -1.f }, { 0.f, 1.f } },
        { { -s, -s, -s }, {  0.f,  0.f, -1.f }, { 1.f, 1.f } },
        { { -s, +s, -s }, {  0.f,  0.f, -1.f }, { 1.f, 0.f } },
        { { +s, +s, -s }, {  0.f,  0.f, -1.f }, { 0.f, 0.f } },

        { { -s, -s, -s }, { -1.f,  0.f,  0.f }, { 0.f, 1.f } },
        { { -s, -s, +s }, { -1.f,  0.f,  0.f }, { 1.f, 1.f } },
        { { -s, +s, +s }, { -1.f,  0.f,  0.f }, { 1.f, 0.f } },
        { { -s, +s, -s }, { -1.f,  0.f,  0.f }, { 0.f, 0.f } },

        { { -s, +s, +s }, {  0.f,  1.f,  0.f }, { 0.f, 1.f } },
        { { +s, +s, +s }, {  0.f,  1.f,  0.f }, { 1.f, 1.f } },
        { { +s, +s, -s }, {  0.f,  1.f,  0.f }, { 1.f, 0.f } },
        { { -s, +s, -s }, {  0.f,  1.f,  0.f }, { 0.f, 0.f } },

        { { -s, -s, -s }, {  0.f, -1.f,  0.f }, { 0.f, 1.f } },
        { { +s, -s, -s }, {  0.f, -1.f,  0.f }, { 1.f, 1.f } },
        { { +s, -s, +s }, {  0.f, -1.f,  0.f }, { 1.f, 0.f } },
        { { -s, -s, +s }, {  0.f, -1.f,  0.f }, { 0.f, 0.f } }
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

    auto vertexFn = NS::TransferPtr(m_DefaultLibrary->newFunction( NS::String::string("vertexMain8", NS::UTF8StringEncoding)));
    auto fragFn = NS::TransferPtr(m_DefaultLibrary->newFunction( NS::String::string("fragmentMain8", NS::UTF8StringEncoding)));
    
    auto pDsDesc = NS::TransferPtr(MTL::DepthStencilDescriptor::alloc()->init());
    pDsDesc->setDepthCompareFunction(MTL::CompareFunction::CompareFunctionLess);
    pDsDesc->setDepthWriteEnabled(true);
    m_DepthStencilState = NS::TransferPtr(m_Device->newDepthStencilState(pDsDesc.get()));

    auto pTextureDesc = NS::TransferPtr(MTL::TextureDescriptor::alloc()->init());
    pTextureDesc->setWidth(kTextureWidth);
    pTextureDesc->setHeight(kTextureHeight);
    pTextureDesc->setPixelFormat(MTL::PixelFormatRGBA8Unorm);
    pTextureDesc->setTextureType(MTL::TextureType2D);
    pTextureDesc->setStorageMode(MTL::StorageModeManaged);
    pTextureDesc->setUsage(MTL::ResourceUsageSample | MTL::ResourceUsageRead | MTL::ResourceUsageWrite);

    m_Texture = NS::TransferPtr(m_Device->newTexture(pTextureDesc.get()));

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
    else
    {
        SetupCompute();
    }
}

void MetalRendererSample8::RenderFrame(MTL::Viewport viewport, MTL::RenderPassDescriptor* renderPassDescriptor, MTL::Drawable* drawable)
{
    if (m_BeginCapture)
    {
        TriggerCapture();
    }
    
    // Metal syncronizes resource access between render and compute
    GenerateTexture();
    
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

    float4x4 rt = MetalMath::makeTranslate( objectPosition );
    float4x4 rr1 = MetalMath::makeYRotate( -m_Angle );
    float4x4 rr0 = MetalMath::makeXRotate( m_Angle * 0.5 );
    float4x4 rtInv = MetalMath::makeTranslate( { -objectPosition.x, -objectPosition.y, -objectPosition.z } );
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

        float4x4 scale = MetalMath::makeScale( (float3){ scl, scl, scl } );
        float4x4 zrot = MetalMath::makeZRotate( m_Angle * sinf((float)ix) );
        float4x4 yrot = MetalMath::makeYRotate( m_Angle * cosf((float)iy));

        float x = ((float)ix - (float)kInstanceRows/2.f) * (2.f * scl) + scl;
        float y = ((float)iy - (float)kInstanceColumns/2.f) * (2.f * scl) + scl;
        float z = ((float)iz - (float)kInstanceDepth/2.f) * (2.f * scl);
        float4x4 translate = MetalMath::makeTranslate( MetalMath::add( objectPosition, { x, y, z } ) );

        pInstanceData[ i ].instanceTransform = fullObjectRot * translate * yrot * zrot * scale;
        pInstanceData[ i ].instanceNormalTransform = MetalMath::discardTranslation( pInstanceData[ i ].instanceTransform );

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
    
    pCameraData->perspectiveTransform = MetalMath::makePerspective( 45.f * M_PI / 180.f, 1.f, 0.03f, 500.0f ) ;
    pCameraData->worldTransform = MetalMath::makeIdentity();
    pCameraData->worldNormalTransform = MetalMath::discardTranslation( pCameraData->worldTransform );
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
    renderEncoder->setFragmentTexture(m_Texture.get(), 0);
    renderEncoder->drawIndexedPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle,
                                    36, MTL::IndexType::IndexTypeUInt16,
                                    m_IndexBuffer.get(),
                                    0,
                                    kNumInstances);
    renderEncoder->popDebugGroup();
    renderEncoder->endEncoding();

    commandBuffer->presentDrawable(drawable);
    commandBuffer->commit();
    
    if (m_BeginCapture)
    {
        MTL::CaptureManager* pCaptureManager = MTL::CaptureManager::sharedCaptureManager();
        pCaptureManager->stopCapture();

        m_BeginCapture = false;
        m_HasCaptured = true;
    }

    // Automattically trigger a capture if person has not used UI to trigger one.
    if ( !m_HasCaptured )
    {
        auto end = std::chrono::system_clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::seconds>(end - start);

        if ( diff.count() > kAutoCaptureTimeoutSecs )
        {
            m_BeginCapture = true;
        }
    }
}

void MetalRendererSample8::ViewportSizeChanged(MTL::Viewport viewport)
{
    
}

void MetalRendererSample8::SetupCompute()
{
    NS::Error* pError = nullptr;
    //auto pMandelbrotFn = m_DefaultLibrary->newFunction( NS::String::string("mandelbrot_set", NS::UTF8StringEncoding));
    
    auto pMandelbrotFn = m_DefaultLibrary->newFunction( NS::String::string("mandelbrot_set_animated", NS::UTF8StringEncoding));
    m_ComputePipelineState = NS::TransferPtr(m_Device->newComputePipelineState( pMandelbrotFn, &pError));
    
    if (pError)
    {
        __builtin_printf("%s", pError->localizedDescription()->utf8String());
        assert(false);
    }
    
    m_TextureAnimationBuffer = NS::TransferPtr(m_Device->newBuffer(sizeof(uint), MTL::ResourceStorageModeManaged));
}

void MetalRendererSample8::GenerateTexture()
{
    // Reference counting these causes a crash
    //auto pCommandBuffer = NS::TransferPtr(m_CommandQueue->commandBuffer());
    //auto pComputeEncoder = NS::TransferPtr(pCommandBuffer->computeCommandEncoder());
    
    auto pCommandBuffer = m_CommandQueue->commandBuffer();
    auto pComputeEncoder = pCommandBuffer->computeCommandEncoder();
    
    uint* ptr = reinterpret_cast<uint*>(m_TextureAnimationBuffer->contents());
    *ptr = (m_AnimationIndex++) % 5000;
    m_TextureAnimationBuffer->didModifyRange(NS::Range::Make(0, sizeof(uint)));

    pComputeEncoder->setComputePipelineState( m_ComputePipelineState.get() );
    pComputeEncoder->setTexture( m_Texture.get(), 0 );
    pComputeEncoder->setBuffer(m_TextureAnimationBuffer.get(), 0, 0);

    MTL::Size gridSize = MTL::Size(kTextureWidth, kTextureHeight, 1);

    NS::UInteger threadGroupSize = m_ComputePipelineState->maxTotalThreadsPerThreadgroup();
    
    MTL::Size threadgroupSize(threadGroupSize, 1, 1 );

    pComputeEncoder->dispatchThreads(gridSize, threadgroupSize);

    pComputeEncoder->endEncoding();

    pCommandBuffer->commit();
}

void MetalRendererSample8::TriggerCapture()
{
    bool success;

    MTL::CaptureManager* pCaptureManager = MTL::CaptureManager::sharedCaptureManager();
    success = pCaptureManager->supportsDestination( MTL::CaptureDestinationGPUTraceDocument);
    
    if ( !success )
    {
        __builtin_printf( "Capture support is not enabled\n");
        assert( false );
    }

    char filename[NAME_MAX];
    std::time_t now;
    std::time( &now );
    std::strftime( filename, NAME_MAX, "capture-%H-%M-%S_%m-%d-%y.gputrace", std::localtime( &now ) );

    m_TraceSaveFilePath = NSTemporaryDirectory()->stringByAppendingString( NS::String::string( filename, NS::UTF8StringEncoding ) );
    auto outPath = m_TraceSaveFilePath->utf8String();
    NS::URL* pURL = NS::URL::alloc()->initFileURLWithPath( m_TraceSaveFilePath );

    MTL::CaptureDescriptor* pCaptureDescriptor = MTL::CaptureDescriptor::alloc()->init();

    pCaptureDescriptor->setDestination( MTL::CaptureDestinationGPUTraceDocument );
    pCaptureDescriptor->setOutputURL( pURL );
    pCaptureDescriptor->setCaptureObject( m_Device.get() );

    NS::Error *pError = nullptr;

    success = pCaptureManager->startCapture( pCaptureDescriptor, &pError );
    
    if ( !success )
    {
        __builtin_printf( "Failed to start capture: \"%s\" for file \"%s\"\n", pError->localizedDescription()->utf8String(),  m_TraceSaveFilePath->utf8String() );
        assert( false );
    }

    pURL->release();
    pCaptureDescriptor->release();
}
