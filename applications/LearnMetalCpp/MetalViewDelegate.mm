//
//  Renderer.m
//  ArTest
//
//  Created by Victor Rykov on 6/20/17.
//  Copyright © 2017 Victor Rykov. All rights reserved.
//

#import "MetalViewDelegate.h"

#include <Thor/Math/Simd/Simd.h>
#include <Thor/Core/Common.h>
#include <Thor/Core/Debug/ThLogger.h>
#include <Metal/Metal.hpp>

#include "MetalRendererTriangle.h"
#include "MetalRendererSample0.h"
#include "MetalRendererSample1.h"
#include "MetalContext.h"

@implementation MetalViewDelegate
{
    MTKView* m_View;    
    NS::SharedPtr<MTL::Device> m_Device;
    NS::SharedPtr<MTL::CommandQueue> m_CommandQueue;
    NS::SharedPtr<MTL::Library> m_DefaultLibrary;
    NS::SharedPtr<MTL::RenderPipelineState> m_PipelineState;
    NS::SharedPtr<MTL::Buffer> m_VertexBuffer;
    
    MetalRenderer* m_Renderer;
}

-(void)dealloc
{
    delete m_Renderer;
}

- (instancetype)initWithView:(MTKView*)view
{
    self = [super init];
    self->m_Renderer = nullptr;
    
    if (self)
    {
        m_View = view;
        [self setupView];
        [self setupRendering];
    }
    
    return self;
}

- (void)setupView
{
    m_Device = MetalContext::DefaultDevice();
    m_View.delegate = self;
    m_View.device = (__bridge id<MTLDevice>)m_Device.get();
    
    // Setup the render target, choose values based on your app.
    m_View.sampleCount = 1;
    m_View.depthStencilPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
    m_View.colorPixelFormat = MTLPixelFormatBGRA8Unorm;
    m_View.clearColor = MTLClearColorMake( 0.0, 0.0, 0.0, 0.0 );
    m_View.clearDepth = 0;
}

- (void)setupRendering
{
    //m_Renderer = new MetalRendererTriangle(m_Device);MetalRendererSample0
    //m_Renderer = new MetalRendererSample0(m_Device);
    m_Renderer = new MetalRendererSample1(m_Device);
    m_Renderer->SetupRendering();
}

- (void)update
{
}

// Called whenever view changes orientation or layout is changed
- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size
{
    MTL::Viewport viewport =
    {
        .originX = 0.0,
        .originY = 0.0,
        .width = size.width,
        .height = size.height,
        .znear = 0.0,
        .zfar = 1.0
    };
    
    m_Renderer->ViewportSizeChanged(viewport);
}


// Called whenever the view needs to render
- (void)drawInMTKView:(nonnull MTKView *)view
{
    NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();
    MTL::Viewport viewport =
    {
        .originX = 0.0,
        .originY = 0.0,
        .width = m_View.drawableSize.width,
        .height = m_View.drawableSize.height,
        .znear = 0.0,
        .zfar = 1.0
    };
    MTL::RenderPassDescriptor* renderPassDescriptor = (__bridge_retained MTL::RenderPassDescriptor*)m_View.currentRenderPassDescriptor;
    MTL::Drawable* drawable = (__bridge MTL::Drawable*)m_View.currentDrawable;
    m_Renderer->RenderFrame(viewport, renderPassDescriptor, drawable);
    pool->release();
}

@end
