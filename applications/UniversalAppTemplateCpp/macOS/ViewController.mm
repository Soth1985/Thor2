//
//  ViewController.m
//  Test
//
//  Created by Victor Rykov on 7/16/16.
//  Copyright © 2016 Victor Rykov. All rights reserved.
//

#import "ViewController.h"
#import "../MetalViewDelegate.h"

#include <Thor/MetalRenderer/ThMetalContext.h>

using namespace Thor;

@implementation ViewController
{
    MetalViewDelegate* m_Delegate;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    MTKView *view = (MTKView *)self.view;
    ThMetalContext::Initialize(ThFramebufferDescriptor());
    m_Delegate = [[MetalViewDelegate alloc] initWithView:view];
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}

@end
