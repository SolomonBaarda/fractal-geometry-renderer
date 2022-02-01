#define CAMERA_POSITIONS_LENGTH 1
#define CAMERA_POSITIONS_ARRAY { (float4)(10, -10, -10, 0) }

#define CAMERA_FACING_DIRECTIONS_LENGTH 1
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(-10, -10, -10)), 0.0f) }

#define SCENE_LIGHT_POSITION (float3)(100, -100, 100)
#define SCENE_LIGHT_COLOUR (float3)(1.0f, 1.0f, 0.98f)


#include "sdf.cl"

float4 signedDistanceEstimation(float3 position, float time)
{
	float distance = planeSDF(position, (float3)(0, 1, 0), 0);

	return (float4)((float3)(0.5f, 0.5f, 0.5f), distance);
}

#include "main.cl"
