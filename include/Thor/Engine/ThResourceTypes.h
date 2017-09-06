#pragma once

#include <Thor/Engine/ThEngineInterface.h>

namespace Thor
{
    struct eResourceKind
    {
        enum Val
        {
            Texture = 1,
            Sampler,
            Buffer,
            PipelineState,
            Geometry,
            Material,
            Last = 128
        };
    };

    struct eResourceType
    {
        enum Val
        {
            Any = eThType::AnyType,
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
            VertexBuffer,
            IndexBuffer,
            //PipelineState
            GraphicsPipelineState,
            ComputePipelineState,
            //Geometry
            MeshGeometry,
            MeshInstance,
            //Material
            Phong,
            Last = 2048
        };
    };
}
