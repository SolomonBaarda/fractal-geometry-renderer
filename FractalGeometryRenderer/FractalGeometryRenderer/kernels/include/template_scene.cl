#define CAMERA_POSITIONS_LENGTH 1
#define CAMERA_POSITIONS_ARRAY { (float4)(0, 0, 0, 0) }

#define CAMERA_FACING_DIRECTIONS_LENGTH 1
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(-10, -10, -10)), 0.0f) }

#include "sdf.cl"

float4 signedDistanceEstimation(float3 position, float time)
{
	float distance = sphereSDF(position, (float3)(0, 0, 0), 3.5f);

	return (float4)((float3)(0.5f, 0.5f, 0.5f), distance);
}

#include "main.cl"
