#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION

// AppKit folder is taken from 

#include <TargetConditionals.h>
#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <MetalFX/MetalFX.hpp>
#include <QuartzCore/QuartzCore.hpp>

#ifdef TARGET_OS_MAC
    //#include <AppKit/AppKit.hpp>
#endif
//#include <MetalKit/MetalKit.hpp>
