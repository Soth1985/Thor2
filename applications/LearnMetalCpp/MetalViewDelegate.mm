//
//  Renderer.m
//  ArTest
//
//  Created by Victor Rykov on 6/20/17.
//  Copyright © 2017 Victor Rykov. All rights reserved.
//

#import "MetalViewDelegate.h"

#include <Thor/SimdMath/Simd.h>
#include <Thor/Core/Common.h>
#include <Thor/Core/Debug/ThLogger.h>
#include <Thor/MetalRenderer/ThMetalUtils.h>
#include <Metal/Metal.hpp>

#include "MetalRendererSample0.h"
#include "MetalRendererSample1.h"
#include "MetalRendererSample2.h"
#include "MetalRendererSample3.h"
#include "MetalRendererSample4.h"
#include "MetalRendererSample5.h"
#include "MetalRendererSample6.h"
#include "MetalRendererSample8.h"

using namespace Thor;

@implementation MetalViewDelegate
{
    MTKView* m_View;    
    NS::SharedPtr<MTL::Device> m_Device;
    ThMetalRenderer* m_Renderer;
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
    m_Device = ThMetalUtils::CreateDefaultDevice();
    m_View.delegate = self;
    m_View.device = (__bridge id<MTLDevice>)m_Device.get();
    
    // Setup the render target, choose values based on your app.
    auto frameBufferDesc = ThFramebufferDescriptor{};
    m_View.sampleCount = frameBufferDesc.m_SampleCount;
    m_View.depthStencilPixelFormat = (MTLPixelFormat)frameBufferDesc.m_DepthStencilPixelFormat;
    m_View.colorPixelFormat = (MTLPixelFormat)frameBufferDesc.m_ColorPixelFormat;
    auto clearColor = frameBufferDesc.m_ClearColor;
    m_View.clearColor = MTLClearColorMake(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    m_View.clearDepth = frameBufferDesc.m_ClearDepth;
    
    auto metalLayer = (CAMetalLayer*)m_View.layer;
    
    if (@available(macOS 13.0, *))
    {
        metalLayer.developerHUDProperties =
        @{
            @"mode": @"default",
            @"logging": @"default"
        };
    }
}

- (void)setupRendering
{
    //m_Renderer = new MetalRendererSample0(m_Device);
    m_Renderer = new MetalRendererSample1(m_Device);
    //m_Renderer = new MetalRendererSample2(m_Device);
    //m_Renderer = new MetalRendererSample3(m_Device);
    //m_Renderer = new MetalRendererSample4(m_Device);
    //m_Renderer = new MetalRendererSample5(m_Device);
    //m_Renderer = new MetalRendererSample6(m_Device);
    //m_Renderer = new MetalRendererSample7(m_Device);
    //m_Renderer = new MetalRendererSample8(m_Device);
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
