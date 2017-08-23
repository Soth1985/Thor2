//
//  ViewController.m
//  Test
//
//  Created by Victor Rykov on 7/16/16.
//  Copyright © 2016 Victor Rykov. All rights reserved.
//

#import "ViewController.h"

#import "../MetalRenderer.h"

@implementation ViewController
{
    MetalRenderer* _renderer;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    MTKView *view = (MTKView *)self.view;
    _renderer = [[MetalRenderer alloc] initWithView:view];
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}

@end
