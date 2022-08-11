//
//  ViewController.m
//  Test
//
//  Created by Victor Rykov on 7/16/16.
//  Copyright © 2016 Victor Rykov. All rights reserved.
//

#import "ViewController.h"
#import "AppDelegate.h"

#import "../MetalViewDelegate.h"

@implementation ViewController
{
    MetalViewDelegate* m_Delegate;
}

- (void)dealloc
{
    delete m_Delegate;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    MTKView* view = (MTKView*)self.view;
    MTK::View* cppView = (__bridge MTK::View*)view;
    AppDelegate* appDelegate = (AppDelegate*)NSApplication.sharedApplication.delegate;
    m_Delegate = new MetalViewDelegate(appDelegate.Device, cppView);
}

- (void)setRepresentedObject:(id)representedObject
{
    [super setRepresentedObject:representedObject];
    // Update the view, if already loaded.
}

@end
