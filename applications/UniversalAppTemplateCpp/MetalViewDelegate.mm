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
#include <Metal/Metal.hpp>

@implementation MetalViewDelegate
{
    MTKView* m_View;
    id <MTLDevice> _device;
    id <MTLCommandQueue> _commandQueue;
    id <MTLLibrary> _defaultLibrary;
    
    id <MTLBuffer> _vertexBuffer;
    id <MTLRenderPipelineState> _pipelineState;
    
    MTL::Device* m_Device;
}

- (instancetype)initWithView:(MTKView*)view {
    self = [super init];
    m_Device = nullptr;
    
    if (self)
    {
        m_View = view;
        [self setupMetal];
        [self setupView];
        [self setupRendering];
    }
    
    return self;
}

-(void)dealloc
{
    m_Device->release();
}

- (void)setupView
{
    m_View.delegate = self;
    m_View.device = _device;
    
    // Setup the render target, choose values based on your app.
    m_View.sampleCount = 1;
    m_View.depthStencilPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
    m_View.colorPixelFormat = MTLPixelFormatBGRA8Unorm;
    m_View.clearColor = MTLClearColorMake( 0.0, 0.0, 0.0, 0.0 );
    m_View.clearDepth = 0;
}

- (void)setupMetal
{
    // Set the view to use the default device.
    _device = MTLCreateSystemDefaultDevice();
    
    // Create a new command queue.
    _commandQueue = [_device newCommandQueue];
    
    // Load all the shader files with a metal file extension in the project.
    _defaultLibrary = [_device newDefaultLibrary];
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
    
    MTLResourceOptions resourceOptions;
#ifdef THOR_PLATFORM_IOS
    resourceOptions = MTLResourceStorageModeShared;
#else
    resourceOptions = MTLResourceStorageModeManaged;
#endif
    _vertexBuffer = [_device newBufferWithBytes:Vertices length:sizeof(Vertices) options:resourceOptions];
    id<MTLFunction> vertexFunc = [_defaultLibrary newFunctionWithName:@"vertexFunc"];
    id<MTLFunction> fragmentFunc = [_defaultLibrary newFunctionWithName:@"fragmentFunc"];
    MTLRenderPipelineDescriptor *renderPipelineDesc = [[MTLRenderPipelineDescriptor alloc] init];
    MTLVertexDescriptor* vertexDesc = [[MTLVertexDescriptor alloc] init];
    
    vertexDesc.attributes[0].format = MTLVertexFormatFloat2;
    vertexDesc.attributes[0].bufferIndex = 0;
    vertexDesc.attributes[0].offset = 0;
    vertexDesc.attributes[1].format = MTLVertexFormatFloat4;
    vertexDesc.attributes[1].bufferIndex = 0;
    vertexDesc.attributes[1].offset = 2 * sizeof(float);
    vertexDesc.layouts[0].stride = 6 * sizeof(float);
    vertexDesc.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
    renderPipelineDesc.vertexFunction = vertexFunc;
    renderPipelineDesc.fragmentFunction = fragmentFunc;
    renderPipelineDesc.vertexDescriptor = vertexDesc;
    renderPipelineDesc.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
    renderPipelineDesc.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
    renderPipelineDesc.stencilAttachmentPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
    
    // Create MTLRenderPipelineState from MTLRenderPipelineDescriptor
    NSError *errors = nil;
    _pipelineState = [_device newRenderPipelineStateWithDescriptor:renderPipelineDesc error:&errors];
}

- (void)reshape
{
}

- (void)update
{
}

- (void)render
{
    id <MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    commandBuffer.label = @"Main Command Buffer";
    
    // Obtain a renderPassDescriptor generated from the view's drawable textures.
    MTLRenderPassDescriptor* renderPassDescriptor = m_View.currentRenderPassDescriptor;
    
    if (renderPassDescriptor == nil)
        return;
    
    // Create a render command encoder so we can render into something.
    id <MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
    
    renderEncoder.label = @"Final Pass Encoder";
    
    // Set context state.
    [renderEncoder setViewport:{0, 0, m_View.drawableSize.width, m_View.drawableSize.height, 0, 1}];
    [renderEncoder pushDebugGroup:@"Render Triangle"];
    
    [renderEncoder setRenderPipelineState:_pipelineState];
    [renderEncoder setVertexBuffer:_vertexBuffer offset:0 atIndex:0];
    [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];
    
    [renderEncoder popDebugGroup];
    
    // We're done encoding commands.
    [renderEncoder endEncoding];
    
    /*
     Call the view's completion handler which is required by the view since
     it will signal its semaphore and set up the next buffer.
     */
    
    [commandBuffer addCompletedHandler:^(id<MTLCommandBuffer> buffer)
     {
         //NSLog(@"Frame done");
     }];
    
    // Schedule a present once the framebuffer is complete using the current drawable.
    [commandBuffer presentDrawable:m_View.currentDrawable];
    
    // Finalize rendering here & push the command buffer to the GPU.
    [commandBuffer commit];
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