//
//  AppDelegate.h
//  Test
//
//  Created by Victor Rykov on 7/16/16.
//  Copyright © 2016 Victor Rykov. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include <Metal/Metal.hpp>

@interface AppDelegate : NSObject <NSApplicationDelegate>

@property(readonly) NS::SharedPtr<MTL::Device> Device;

@end

