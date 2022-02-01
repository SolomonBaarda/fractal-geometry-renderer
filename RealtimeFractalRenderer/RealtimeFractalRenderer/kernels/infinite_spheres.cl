#define MAXIMUM_MARCH_STEPS 200
#define MAXIMUM_MARCH_DISTANCE 1000.0f

#define CAMERA_POSITIONS_LENGTH 1
#define CAMERA_POSITIONS_ARRAY { (float4)(500, 500, 500, 0) }

#define CAMERA_FACING_DIRECTIONS_LENGTH 1
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(-0.5, -0.5, -0.5)), 0.0f) }

#define SCENE_LIGHT_POSITION (float3)(500, 500, 500)

#include "sdf.cl"

float4 signedDistanceEstimation(float3 position, float time)
{
	const float3 repetition = (float3)(10.0f, 10.0f, 10.0f);
	const float3 repetition_half = repetition * 0.5f;

	// Transform the position in space
	float3 transformed_position = mod(position + repetition_half, repetition) - repetition_half;

	// Calculate the distance to the transformed sphere
	float distance = sphereSDF(transformed_position, (float3)(0, 0, 0), 1.0f);

	//float3 pos = (float3)(mod(position.x, 1.0f) - 0.5f, mod(position.y, 1.0f) - 0.5f, position.z);
	//float distance = magnitude(pos) - 0.3f;

	return (float4)((float3)(0.5f), distance);
}

#include "main.cl"
