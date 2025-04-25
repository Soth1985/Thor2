//
//  ViewController.m
//  Test
//
//  Created by Victor Rykov on 7/16/16.
//  Copyright © 2016 Victor Rykov. All rights reserved.
//

#import "ViewController.h"
#import "../MetalViewDelegate.h"

@implementation ViewController
{
    MetalViewDelegate* m_Delegate;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    MTKView *view = (MTKView *)self.view;
    m_Delegate = [[MetalViewDelegate alloc] initWithView:view];
}

- (void)setRepresentedObject:(id)representedObject {
    [super setRepresentedObject:representedObject];

    // Update the view, if already loaded.
}

@end
