#define MAXIMUM_MARCH_STEPS 200
#define MAXIMUM_MARCH_DISTANCE 1000.0f

#define BENCHMARK_START_STOP_TIME (float2)(1.0f, 16.0f)

#define CAMERA_POSITIONS_LENGTH 4
#define CAMERA_POSITIONS_ARRAY { (float4)(1005, 1005, 1005, 1), (float4)(1105, 1005, 1005, 6), (float4)(1505, 1005, 1005, 11), (float4)(1505, 1005, 1005, 16) }

#define CAMERA_FACING_DIRECTIONS_LENGTH 4
#define CAMERA_FACING_DIRECTIONS_ARRAY { \
	(float4)(normalise((float3)(-1, 0, 0)), 12), \
	(float4)(normalise((float3)(-1, 0, -0.5)), 13), \
	(float4)(normalise((float3)(-0.5, 0, -1)), 14), \
	(float4)(normalise((float3)(-0, 0, -1)), 15) \
	}

#define CAMERA_DO_LOOP false


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
