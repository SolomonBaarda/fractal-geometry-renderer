#include "sdf.cl"

float4 signedDistanceEstimation(float3 position, float time)
{
	float offset = -sin(time * 0.5f) * 2.0f;

	float distance = min(
		sphereSDF(position, (float3)(0, offset, 0), 3.5f),
		boxSDF(position, (float3)(0, offset, 0), (float3)(4, 0.5f, 4))
	);

	return (float4)((float3)(0.5f, 0.5f, 0.5f), distance);
}


#define CAMERA_POSITIONS_LENGTH 3
#define CAMERA_POSITIONS_ARRAY { (float4)(-10, -10, -10, 5), (float4)(-10, -20, -10, 7.5), (float4)(-10, -30, -10, 10) }

#define CAMERA_FACING_DIRECTIONS_LENGTH 2
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(-10, -10, -10)), 5), (float4)(normalise((float3)(-10, -30, -10)), 10) }



#include "main.cl"
