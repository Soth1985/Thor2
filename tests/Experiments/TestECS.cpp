#include <Thor/SimdMath/Simd.h>
#include <Thor/ECS/ThECS.h>

int main()
{
    Thor::ThCore::Initialize();
    simd::float4 vec1 = {1.0, 2.0, 3.0, 4.0};
    simd::float4 vec2 = {5.0, 6.0, 7.0, 8.0};
    
    simd::float4 vec3 = vec1 + vec2;
    float dot = simd::dot(vec1, vec2);
	return 0;
}
