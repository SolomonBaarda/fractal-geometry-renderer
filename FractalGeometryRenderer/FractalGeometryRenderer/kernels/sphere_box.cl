#define CAMERA_POSITIONS_LENGTH 1
#define CAMERA_POSITIONS_ARRAY { (float4)(10, -10, -10, 0) }

#define CAMERA_FACING_DIRECTIONS_LENGTH 1
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(-10, -10, -10)), 0.0f) }

#define SCENE_LIGHT_POSITION (float3)(100, -100, 100)
#define SCENE_LIGHT_COLOUR (float3)(1.0f, 1.0f, 1.0f)


#define SCENE_BACKGROUND_COLOUR (float3)(0.78f, 0.78f, 0.73f)

#define SURFACE_INTERSECTION_EPSILON 0.00001f

#define MAXIMUM_MARCH_STEPS 200
#define MAXIMUM_MARCH_DISTANCE 50.0f


#define DO_SOFT_SHADOWS
#define DO_LAMBERTIAN_REFLECTANCE


#include "sdf.cl"

float4 signedDistanceEstimation(float3 position, float time)
{
	float offset = -sin(time * 0.5f) * 1.0f;

	float distance = opSmoothUnion(
		sphereSDF(position, (float3)(0.0f, offset, 0.0f), 3.5f),
		boxSDF(position, (float3)(0.0f, offset, 0.0f), (float3)(4.0f, 0.5f, 4.0f)),
		(-sin(time * 0.5f) + 1.0f ) * 1.5f
	);

	return (float4)((float3)(0.92f, 0.30f, 0.16f), distance);
}

#include "main.cl"
