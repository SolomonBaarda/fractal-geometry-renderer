#define MAXIMUM_MARCH_STEPS 200
#define MAXIMUM_MARCH_DISTANCE 1000.0f

#define SCENE_LIGHT_POSITION (float3)(500, 500, 500)
#define SCENE_BACKGROUND_COLOUR (float3)(0.1f, 0.1f, 0.1f)

#define DO_LAMBERTIAN_REFLECTANCE true

#define BENCHMARK_START_STOP_TIME (float2)(1.0f, 31.0f)
#define CAMERA_DO_LOOP false

#define CAMERA_POSITIONS_LENGTH 4
#define CAMERA_POSITIONS_ARRAY { (float4)(1005, 1005, 1005, 1), (float4)(1005, 1005, 1025, 11), (float4)(1005, 1005, 1025, 23), (float4)(1055, 965, 975, 31) }

#define CAMERA_FACING_DIRECTIONS_LENGTH 2
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(0, 0, -1)), 12), (float4)(normalise((float3)(-1, 0, 1)), 22) }

#define SCALE 0.05f
#define TIME_SCALE 0.33f

#define REPETITION (float3)(10.0f, 10.0f, 10.0f)
#define REPETITION_HALF REPETITION / 2


#define FORCE_FREE_CAMERA false
#define CAMERA_SPEED 10.0f


#include "sdf.cl"
#include "simplexnoise1234.c"


float4 signedDistanceEstimation(float3 position, float time)
{
	// Transform the position in space
	float3 transformed_position = mod(position + REPETITION_HALF, REPETITION) - REPETITION_HALF;

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
