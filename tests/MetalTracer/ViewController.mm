//
//  ViewController.m
//  Test
//
//  Created by Victor Rykov on 7/16/16.
//  Copyright © 2016 Victor Rykov. All rights reserved.
//

#import "ViewController.h"
#import <Metal/Metal.h>
#include "RayTracer.h"

using namespace Thor;

@implementation ViewController
{
    MTKView* _view;
    id <MTLDevice> _device;
    id <MTLCommandQueue> _commandQueue;
    id <MTLLibrary> _defaultLibrary;
    
    id <MTLBuffer> _vertexBuffer;
    id <MTLBuffer> _indexBuffer;
    id <MTLTexture> _frame;
    id <MTLRenderPipelineState> _pipelineState;
    
    
    RayTracer _rayTracer;
}

- (void)viewDidLoad {
    [super viewDidLoad];

    [self setupMetal];
    [self setupView];
    [self setupRendering];
    
}

- (void)setupView {
    _view = (MTKView *)self.view;
    _view.delegate = self;
    _view.device = _device;
    
    // Setup the render target, choose values based on your app.
    _view.sampleCount = 1;
    _view.depthStencilPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
    _view.colorPixelFormat = MTLPixelFormatBGRA8Unorm;
    _view.clearColor = MTLClearColorMake( 0.0, 0.0, 0.0, 0.0 );
    _view.clearDepth = 0;
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
    /*float Vertices[] =
    {
        -1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, -1.0f,
        1.0f, 1.0f,
        -1.0f, -1.0f,
        0.0f, 1.0f
    };*/
    
    float Vertices[] =
    {
        -1.0f, 1.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, -1.0f,
        1.0f, 0.0f,
        -1.0f, -1.0f,
        0.0f, 0.0f
    };
    
    short Indices[] =
    {
        0, 1, 2, 2, 3, 0
    };
    
    _vertexBuffer = [_device newBufferWithBytes:Vertices length:sizeof(Vertices) options:MTLResourceStorageModeManaged];
    _indexBuffer = [_device newBufferWithBytes:Indices length:sizeof(Indices) options:MTLResourceStorageModeManaged];
    
    MTLTextureDescriptor *texDesc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm
                                                          width:_view.drawableSize.width
                                                          height:_view.drawableSize.height
                                                          mipmapped:NO];
    _frame = [_device newTextureWithDescriptor:texDesc];
    
    RayTracerOptions options;
    options.m_Width = _view.drawableSize.width;
    options.m_Height = _view.drawableSize.height;
    options.m_SamplesPerPixel = 10;
    options.m_TraceDepth = 20;
    options.m_CameraMode = CameraMode::LensDefocus;
    options.m_CameraOrigin = ThVec3f(2.0, 2.0, 1.0);
    options.m_CameraFov = 20.0;
    options.m_CameraAperture = 4.0;
    Scene* scene = new Scene();
    Object sphere1 = scene->CreateObject();
    Object sphere2 = scene->CreateObject();
    Object sphere3 = scene->CreateObject();
    Object sphere4 = scene->CreateObject();
    Object sphere5 = scene->CreateObject();
    ComponentRef shape1 = scene->AddSphereShape(sphere1, ThSpheref(ThVec3f(0.0f,0.0f,-1.0f), 0.5f));
    ComponentRef shape2 = scene->AddSphereShape(sphere2, ThSpheref(ThVec3f(0.0f,-100.5f,-1.0f), 100.0f));
    ComponentRef shape3 = scene->AddSphereShape(sphere3, ThSpheref(ThVec3f(1.0f,0.0f,-1.0f), 0.5f));
    ComponentRef shape4 = scene->AddSphereShape(sphere4, ThSpheref(ThVec3f(-1.0f,0.0f,-1.0f), 0.5f));
    ComponentRef shape5 = scene->AddSphereShape(sphere4, ThSpheref(ThVec3f(-1.0f,0.0f,-1.0f), -0.45f));
    ComponentRef material1 = scene->AddLambertMaterial(sphere1, ThVec3f(0.8,0.3,0.3));
    ComponentRef material2 = scene->AddLambertMaterial(sphere2, ThVec3f(0.8,0.8,0.0));
    ComponentRef material3 = scene->AddMetalMaterial(sphere3, ThVec3f(0.8,0.6,0.2), 0.3);
    ComponentRef material4 = scene->AddDielectricMaterial(sphere4, 1.5);
    ComponentRef material5 = scene->AddDielectricMaterial(sphere5, 1.5);
    scene->SetMaterial(shape1, material1);
    scene->SetMaterial(shape2, material2);
    scene->SetMaterial(shape3, material3);
    scene->SetMaterial(shape4, material4);
    scene->SetMaterial(shape5, material5);
    _rayTracer.Init(options, scene);
    
    id<MTLFunction> vertexFunc = [_defaultLibrary newFunctionWithName:@"vertexFunc"];
    id<MTLFunction> fragmentFunc = [_defaultLibrary newFunctionWithName:@"fragmentFunc"];
    MTLRenderPipelineDescriptor *renderPipelineDesc = [[MTLRenderPipelineDescriptor alloc] init];
    MTLVertexDescriptor* vertexDesc = [[MTLVertexDescriptor alloc] init];
    
    vertexDesc.attributes[0].format = MTLVertexFormatFloat2;
    vertexDesc.attributes[0].bufferIndex = 0;
    vertexDesc.attributes[0].offset = 0;
    vertexDesc.attributes[1].format = MTLVertexFormatFloat2;
    vertexDesc.attributes[1].bufferIndex = 0;
    vertexDesc.attributes[1].offset = 2 * sizeof(float);
    vertexDesc.layouts[0].stride = 4 * sizeof(float);
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
    MTLTextureDescriptor *texDesc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm
                                                          width:_view.drawableSize.width
                                                          height:_view.drawableSize.height
                                                          mipmapped:NO];
    _frame = [_device newTextureWithDescriptor:texDesc];
}

- (void)update
{
}

- (void)render
{
    if (_rayTracer.GetState() == RayTracerState::Idle)
    {
        _rayTracer.RenderFrame();
    }
    else if (_rayTracer.GetState() == RayTracerState::FrameReady)
    {
        const Film* film = _rayTracer.GetFilm();
        const RayTracerOptions& options = _rayTracer.GetOptions();
        ThI32 width = _view.drawableSize.width;
        ThI32 height = _view.drawableSize.height;
        
        if (options.m_Width == width && options.m_Height == height)
        {
            MTLRegion region = MTLRegionMake2D(0, 0, width, height);
            
            [_frame replaceRegion:region
                    mipmapLevel:0
                    withBytes:film->GetFrame()
                    bytesPerRow:width * 4];
            
            _rayTracer.FrameFetched();
        }
        else
        {
            _rayTracer.FrameFetched();
            _rayTracer.ResizeFilm(width, height);
        }
        
    }
    
    id <MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    commandBuffer.label = @"Main Command Buffer";
    
    // Obtain a renderPassDescriptor generated from the view's drawable textures.
    MTLRenderPassDescriptor* renderPassDescriptor = _view.currentRenderPassDescriptor;
    
    // Create a render command encoder so we can render into something.
    id <MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
    
    renderEncoder.label = @"Final Pass Encoder";
    
    // Set context state.
    [renderEncoder setViewport:{0, 0, _view.drawableSize.width, _view.drawableSize.height, 0, 1}];
    [renderEncoder pushDebugGroup:@"Render Quad"];
    
    [renderEncoder setRenderPipelineState:_pipelineState];
    [renderEncoder setVertexBuffer:_vertexBuffer offset:0 atIndex:0];
    [renderEncoder setFragmentTexture:_frame atIndex:0];
    [renderEncoder drawIndexedPrimitives:MTLPrimitiveTypeTriangle indexCount:6 indexType:MTLIndexTypeUInt16 indexBuffer:_indexBuffer indexBufferOffset:0];
    //[renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:3];
    
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
    [commandBuffer presentDrawable:_view.currentDrawable];
    
    // Finalize rendering here & push the command buffer to the GPU.
    [commandBuffer commit];
}

- (void)setRepresentedObject:(id)representedObject
{
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}

// Called whenever view changes orientation or layout is changed
- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size
{
    [self reshape];
}


// Called whenever the view needs to render
- (void)drawInMTKView:(nonnull MTKView *)view
{
    @autoreleasepool {
        [self render];
    }
}

@end
