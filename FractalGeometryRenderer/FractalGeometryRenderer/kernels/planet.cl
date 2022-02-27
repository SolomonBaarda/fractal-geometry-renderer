#define CAMERA_POSITIONS_LENGTH 1
#define CAMERA_POSITIONS_ARRAY { (float4)(-10, -10, 10, 0) }

#define CAMERA_FACING_DIRECTIONS_LENGTH 1
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(-10, -10, -10)), 0.0f) }

#define SCENE_LIGHT_POSITION (float3)(0, -100, 0)
#define SCENE_LIGHT_COLOUR (float3)(1.0f, 1.0f, 0.98f)

#define SCENE_BACKGROUND_COLOUR (float3)(0.5f, 0.8f, 0.9f)

#define MAXIMUM_MARCH_STEPS 200
#define MAXIMUM_MARCH_DISTANCE 200.0f
#define SURFACE_INTERSECTION_EPSILON 0.00001f

#define CAMERA_FOCUS_DISTANCE 0.01f


#define FORCE_FREE_CAMERA
#define CAMERA_SPEED 5.0f

#define DO_LAMBERTIAN_REFLECTANCE
//#define DO_SOFT_SHADOWS


#include "sdf.cl"
#include "simplexnoise1234.c"

#define SCALE 0.25f
#define AMPLITUDE 10.0f
#define ITERATIONS 3

#define FREQUENCY_MULTIPLIER 2
#define AMPLITUDE_MULTIPLIER 0.4f

float getHeightAt(const float x, const float y, const float z)
{
	float frequency = SCALE;
	float amplitude = AMPLITUDE;
	float height = 0.0f;

	for (int j = 0; j < ITERATIONS; j++)
	{
		height += amplitude * snoise3(x * frequency, y * frequency, z * frequency);
		frequency *= FREQUENCY_MULTIPLIER;
		amplitude *= AMPLITUDE_MULTIPLIER;
	}

	return height;
}

float4 signedDistanceEstimation(float3 position, float time)
{
	float height = getHeightAt(position.x, position.y, position.z) - 1.0f;
	float distance = sphereSDF(position, (float3)(0), 5.0f) - max(height, 0.0f) / 20.0f;

	float3 colour;

	// Water
	if (height <= 0.0f)
	{
		colour = (float3)(0, 0.05, 0.8);
	}
	// Land
	else
	{
		// Snow
		if (height >= 8.0f)
		{
			colour = (float3)(0.9, 0.9, 1);
		}
		// Grass
		else
		{
			colour = (float3)(0.1, 0.5, 0.2);
		}
	}

	return (float4)(colour, distance);
}

#include "main.cl"
