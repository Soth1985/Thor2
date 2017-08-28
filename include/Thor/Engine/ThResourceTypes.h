#pragma once

#include <Thor/Engine/ThEngineInterface.h>

namespace Thor
{
    struct eResourceKind
    {
        enum Val
        {
            Texture,
            Sampler,
            Buffer,
            PipelineState,
            Last = 128
        };
    };

    struct eResourceType
    {
        enum Val
        {
            //Texture
            Texture1D,
            Texture2D,
            Texture2DMultisample,
            Texture3D,
            TextureCube,
            Texture1DArray,
            Texture2DArray,
            Texture3DArray,
            TextureCubeArray,
            //Sampler
            SamplerState,
            //Buffer
            Phong,
            //Light
            PointLight,
            DirectionalLight,
            SpotLight,
            AreaLight,
            //Camera
            PinholeCamera,
            Last = 2048
        };
    };
}
