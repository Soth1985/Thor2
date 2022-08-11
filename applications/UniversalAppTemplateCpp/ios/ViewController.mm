//
//  ViewController.m
//  ArTest
//
//  Created by Victor Rykov on 6/20/17.
//  Copyright © 2017 Victor Rykov. All rights reserved.
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
