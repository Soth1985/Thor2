//
//  ViewController.m
//  ArTest
//
//  Created by Victor Rykov on 6/20/17.
//  Copyright © 2017 Victor Rykov. All rights reserved.
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
    
    UITapGestureRecognizer *tapGesture = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleTap:)];
    NSMutableArray *gestureRecognizers = [NSMutableArray array];
    [gestureRecognizers addObject:tapGesture];
    [gestureRecognizers addObjectsFromArray:view.gestureRecognizers];
    view.gestureRecognizers = gestureRecognizers;
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

- (void)handleTap:(UIGestureRecognizer*)gestureRecognize
{
    
}

@end
