#include "utils.cl"

float4 signedDistanceEstimation(float3 position, float time)
{
	float offset = -sin(time * 0.5f) * 2.0f;

	float distance = min(
		sphereSDF(position, (float3)(0, offset, 0), 3.5f),
		boxSDF(position, (float3)(0, offset, 0), (float3)(4, 0.5f, 4))
	);

	return (float4)((float3)(0.5f, 0.5f, 0.5f), distance);
}

#include "main.cl"
