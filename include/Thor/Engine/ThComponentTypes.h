#pragma once

namespace Thor
{
    struct eComponentKind
    {
        enum Val
        {
            Location = 1,
            GeometryShape,
            Light,
            Camera,
            CollisionShape,
            PhysicsActor,
            ParticleSystem,
            Script,
            Utility,
            Last = 128
        };
    };

    struct eComponentType
    {
        enum Val
        {
            Any = eThType::AnyType,
            //Location
            Transform,
            //GeometryShape
            MeshGeometry,
            SpriteGeometry,
            NurbsGeometry,
            CatmullClarkGeometry,
            BoxGeometry,
            SphereGeometry,
            CylinderGeometry,
            //Light
            PointLight,
            DirectionalLight,
            SpotLight,
            AreaLight,
            //Camera
            PinholeCamera,
            //Utility
            Tag,
            Last = 2048
        };
    };
}
