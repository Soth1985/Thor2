#pragma once

#include <Thor/Engine/ThEngineInterface.h>

namespace Thor
{
    struct eComponentKind
    {
        enum Val
        {
            Location = 1,
            GeometryShape,
            Light,
            Material,
            Camera,
            CollisionShape,
            PhysicsActor,
            ParticleSystem,
            Script,
            Last = 128
        };
    };

    struct eComponentType
    {
        enum Val
        {
            //Location
            Transform = 1,
            //GeometryShape
            MeshGeometry,
            SpriteGeometry,
            NurbsGeometry,
            CatmullClarkGeometry,
            BoxGeometry,
            SphereGeometry,
            CylinderGeometry,
            //Material
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
