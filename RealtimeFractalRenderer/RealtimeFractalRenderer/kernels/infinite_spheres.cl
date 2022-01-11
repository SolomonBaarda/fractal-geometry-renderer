#define MAXIMUM_MARCH_STEPS 200
#define MAXIMUM_MARCH_DISTANCE 1000.0f

#include "sdf.cl"

float4 signedDistanceEstimation(float3 position, float time)
{
	const float3 repetition = (float3)(10.0f, 10.0f, 10.0f);
	const float3 repetition_half = repetition * 0.5f;

	// Transform the position in space
	float3 transformed_position = mod(position + repetition_half, repetition) - repetition_half;

	// Calculate the distance to the transformed sphere
	float distance = sphereSDF(transformed_position, (float3)(0, 0, 0), 1.0f);

	return (float4)((float3)(0.5f), distance);
}

#include "main.cl"
