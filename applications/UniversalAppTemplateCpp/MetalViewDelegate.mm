//
//  Renderer.m
//  ArTest
//
//  Created by Victor Rykov on 6/20/17.
//  Copyright © 2017 Victor Rykov. All rights reserved.
//

#import "MetalViewDelegate.h"
#import <Metal/Metal.h>

#include <Thor/Math/Simd/Simd.h>
#include <Thor/Core/Common.h>
#include <Metal/Metal.hpp>

#include "MetalRenderer.h"
#include "MetalContext.h"

#include <Thor/Core/Debug/ThLogger.h>

@implementation MetalViewDelegate
{
    MTKView* m_View;
    
    NS::SharedPtr<MTL::Device> m_Device;
    NS::SharedPtr<MTL::CommandQueue> m_CommandQueue;
    NS::SharedPtr<MTL::Library> m_DefaultLibrary;
    NS::SharedPtr<MTL::RenderPipelineState> m_PipelineState;
    
    NS::SharedPtr<MTL::Buffer> m_VertexBuffer;
}

- (instancetype)initWithView:(MTKView*)view
{
    self = [super init];
    
    if (self)
    {
        m_View = view;
        [self setupMetal];
        [self setupView];
        [self setupRendering];
    }
    
    return self;
}

- (void)setupView
{
    m_View.delegate = self;
    m_View.device = (__bridge id<MTLDevice>)m_Device.get();
    
    // Setup the render target, choose values based on your app.
    m_View.sampleCount = 1;
    m_View.depthStencilPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
    m_View.colorPixelFormat = MTLPixelFormatBGRA8Unorm;
    m_View.clearColor = MTLClearColorMake( 0.0, 0.0, 0.0, 0.0 );
    m_View.clearDepth = 0;
}

- (void)setupMetal
{
    m_Device = MetalContext::DefaultDevice();
    m_CommandQueue = NS::TransferPtr(m_Device->newCommandQueue());
    m_DefaultLibrary = NS::TransferPtr(m_Device->newDefaultLibrary());
}

- (void)setupRendering
{
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

- (void)reshape
{
}

- (void)update
{
}

- (void)render
{
    NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();

    MTL::CommandBuffer* commandBuffer = m_CommandQueue->commandBuffer();
    commandBuffer->setLabel(NS::String::string("Main Command Buffer", NS::UTF8StringEncoding));
    
    commandBuffer->addCompletedHandler( ^void( MTL::CommandBuffer* pCmd )
    {
        
    });

    MTL::RenderPassDescriptor* renderPassDescriptor = (__bridge_retained MTL::RenderPassDescriptor*)m_View.currentRenderPassDescriptor;
    MTL::RenderCommandEncoder* renderEncoder = commandBuffer->renderCommandEncoder(renderPassDescriptor);
    renderEncoder->setLabel(NS::String::string("Final Pass Encoder", NS::UTF8StringEncoding));
    MTL::Viewport viewport =
    {
        .originX = 0.0,
        .originY = 0.0,
        .width = m_View.drawableSize.width,
        .height = m_View.drawableSize.height,
        .znear = 0.0,
        .zfar = 1.0
    };
    renderEncoder->setViewport(viewport);
    renderEncoder->pushDebugGroup(NS::String::string("Render Triangle", NS::UTF8StringEncoding));
    renderEncoder->setRenderPipelineState(m_PipelineState.get());
    renderEncoder->setVertexBuffer(m_VertexBuffer.get(), 0, 0);
    renderEncoder->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(3));
    renderEncoder->popDebugGroup();
    renderEncoder->endEncoding();

    MTL::Drawable* drawable = (__bridge MTL::Drawable*)m_View.currentDrawable;
    commandBuffer->presentDrawable(drawable);
    commandBuffer->commit();

    pool->release();
}

// Called whenever view changes orientation or layout is changed
- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size
{
    [self reshape];
}


// Called whenever the view needs to render
- (void)drawInMTKView:(nonnull MTKView *)view
{
    @autoreleasepool
    {
        [self render];
    }
}

@end
