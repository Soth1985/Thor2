//
//  Renderer.h
//  ArTest
//
//  Created by Victor Rykov on 6/20/17.
//  Copyright © 2017 Victor Rykov. All rights reserved.
//

#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>

@interface MetalRenderer : NSObject<MTKViewDelegate>

- (instancetype)initWithView:(MTKView*)view;

@end
