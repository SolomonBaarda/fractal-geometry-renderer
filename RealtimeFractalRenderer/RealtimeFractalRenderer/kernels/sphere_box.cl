#define CAMERA_POSITIONS_LENGTH 1
#define CAMERA_POSITIONS_ARRAY { (float4)(10, -10, -10, 0) }

#define CAMERA_FACING_DIRECTIONS_LENGTH 1
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(-10, -10, -10)), 0.0f) }

#define SCENE_LIGHT_POSITION (float3)(100, -100, 100)


#include "sdf.cl"

float4 signedDistanceEstimation(float3 position, float time)
{
	float offset = -sin(time * 0.5f) * 2.0f;

	float distance = min(
		sphereSDF(position, (float3)(0.0f, offset, 0.0f), 3.5f),
		boxSDF(position, (float3)(0.0f, offset, 0.0f), (float3)(4.0f, 0.5f, 4.0f))
	);

	return (float4)((float3)(0.5f, 0.5f, 0.5f), distance);
}

#include "main.cl"
