//
//  main.m
//  Test
//
//  Created by Victor Rykov on 7/16/16.
//  Copyright © 2016 Victor Rykov. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include <Thor/Core/ThCore.h>

int main(int argc, const char * argv[])
{
    Thor::ThCore::Initialize();
    int result = NSApplicationMain(argc, argv);
    Thor::ThCore::Shutdown();
    return result;
}
