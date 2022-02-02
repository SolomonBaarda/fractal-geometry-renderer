#define MAXIMUM_MARCH_STEPS 200
#define MAXIMUM_MARCH_DISTANCE 1000.0f

#define CAMERA_POSITIONS_LENGTH 1
#define CAMERA_POSITIONS_ARRAY { (float4)(500, 500, 500, 0) }

#define CAMERA_FACING_DIRECTIONS_LENGTH 1
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(-0.5, -0.5, -0.5)), 0.0f) }

#define SCENE_LIGHT_POSITION (float3)(500, 500, 500)

#define SCENE_BACKGROUND_COLOUR (float3)(0.1f, 0.1f, 0.1f)

//#define DO_SOFT_SHADOWS
#define DO_LAMBERTIAN_REFLECTANCE

//#define DO_GLOW

#include "sdf.cl"

#define SCALE 0.05f
#define TIME_SCALE 0.33f

#include "simplexnoise1234.c"


float4 signedDistanceEstimation(float3 position, float time)
{
	const float3 repetition = (float3)(10.0f, 10.0f, 10.0f);
	const float3 repetition_half = repetition * 0.5f;

	// Transform the position in space
	float3 transformed_position = mod(position + repetition_half, repetition) - repetition_half;

	// Calculate the distance to the transformed sphere
	float distance = sphereSDF(transformed_position, (float3)(0, 0, 0), 1.0f);

	// Calculate a point to sample the noise from, based on position and time
	float3 samplePoint = position * SCALE + time * TIME_SCALE;
	// Calculate colour, range -1 to 1 for x, y, and z
	float3 colour = (float3)(snoise1(samplePoint.x) + 1, snoise1(samplePoint.y) + 1, snoise1(samplePoint.z) + 1);

	// Divide colour by 2 so that it is in the range 0 to 1
	return (float4)(colour / 2, distance);
}

#include "main.cl"
