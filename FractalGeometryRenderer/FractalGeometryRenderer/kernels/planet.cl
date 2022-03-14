#ifndef BENCHMARK

#define CAMERA_POSITIONS_LENGTH 1
#define CAMERA_POSITIONS_ARRAY { (float4)(40, 0, 0, 0) }
#define CAMERA_FACING_DIRECTIONS_LENGTH 1
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(1, 1, 1)), 0) }
#define FORCE_FREE_CAMERA true
#define CAMERA_SPEED 5.0f

#define USE_BOUNDING_VOLUME true
//#define DISPLAY_BOUNDING_VOLUME true

#endif

#define SCENE_BACKGROUND_COLOUR (float3)(0.5f, 0.8f, 0.9f)
#define MAXIMUM_MARCH_STEPS 200
#define MAXIMUM_MARCH_DISTANCE 200.0f
#define SURFACE_INTERSECTION_EPSILON 0.00001f
#define CAMERA_FOCUS_DISTANCE 0.01f

#define DO_GEOMETRY_GLOW true
#define SCENE_GLOW_COLOUR (float3)(0.6f, 0.6f, 0.8f)
#define SCENE_MAX_GLOW_DISTANCE 1.0f

#include "simplexnoise1234.c"
#include "types.cl"
#include "sdf.cl"

#define SCALE 0.25f
#define AMPLITUDE 10.0f
#define ITERATIONS 2

#define FREQUENCY_MULTIPLIER 2.5f
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

Light getLight(float time)
{
	float t = fmod(time * 0.5f, 2.0f * PI);

	Light light;
	light.ambient = (float3)(0.1f, 0.1f, 0.1f);
	light.diffuse = (float3)(0.6f, 0.6f, 0.6f);
	light.specular = (float3)(1.0f, 1.0f, 1.0f);
	light.position = (float3)(50 * cos(t), -25, 50 * sin(t));

	return light;
}

Material SDF(const float3 position, const float time, float* distance)
{
	float height = getHeightAt(position.x, position.y, position.z) - 1.0f;

	// Distance estimation
	*distance = sphereSDF(position, (float3)(0), 10.0f) - max(height, 0.0f) / 20.0f;

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

	// Material
	Material material;
	material.ambient = colour;
	material.diffuse = material.ambient;
	material.specular = (float3)(0.5f, 0.5f, 0.5f);
	material.shininess = 25.0f;

	return material;
}

Material getMaterial(float3 position, float time)
{
	float distance;
	return SDF(position, time, &distance);
}

float boundingVolumeDE(float3 position, float time)
{
	return sphereSDF(position, (float3)(0), 11.0f);
}

float DE(float3 position, float time)
{
	float distance;
	SDF(position, time, &distance);
	return distance;
}

#include "main.cl"
