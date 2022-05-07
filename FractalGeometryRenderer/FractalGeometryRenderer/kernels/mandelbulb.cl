#ifndef BENCHMARK

// Config
#define CAMERA_POSITIONS_LENGTH 1
#define CAMERA_POSITIONS_ARRAY { (float4)(2, 0, 0, 0) }
#define CAMERA_FACING_DIRECTIONS_LENGTH 1
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(1, 1, 1)), 0) }
#define FORCE_FREE_CAMERA true

// Features
#define DO_GEOMETRY_GLOW true
#define DO_SOFT_SHADOWS true
#define DO_HARD_SHADOWS false

#ifndef USE_BOUNDING_VOLUME
#define USE_BOUNDING_VOLUME true
#endif

// Debug
#define DO_RENDER_SURFACE_NORMALS false
#define DO_RENDER_MARCHING_ITERATIONS false
#define DISPLAY_BOUNDING_VOLUME false

#endif

#ifndef OVERRIDE
#define OVERRIDE false
#endif

// Scene
#define MAXIMUM_MARCH_STEPS 150
#define MAXIMUM_MARCH_DISTANCE 50.0f
#define SURFACE_INTERSECTION_EPSILON 0.001f
#define SCENE_GLOW_COLOUR (float3)(0.8f, 0.8f, 0.8f)
#define SCENE_MAX_GLOW_DISTANCE 0.05f
#define SCENE_BACKGROUND_COLOUR (float3)(0.1f, 0.1f, 0.1f)
#define CAMERA_SPEED 0.5f
#define SURFACE_SHADOW_EPSILON 0.01f
#define SURFACE_SHADOW_FALLOFF 10.0f

#ifndef ITERATIONS
#define ITERATIONS 10
#endif

#include "utils.cl"
#include "types.cl"
#include "sdf.cl"

#if ! OVERRIDE

Light getLight(float time)
{
	float t = fmod(time * 0.5f, 2.0f * PI);

	Light light;
	light.ambient = (float3)(0.1f, 0.1f, 0.1f);
	light.diffuse = (float3)(0.6f, 0.6f, 0.6f);
	light.specular = (float3)(1.0f, 1.0f, 1.0f);
	light.position = (float3)(5 * cos(t), -5, 5 * sin(t));

	return light;
}

Material mandelbulbSDF(const float3 position, const float time, float* distance)
{
	// Material
	Material material;

	const float power = 7.75f + time * 0.01f;

	float3 w = position;
	float m = dot(w, w);
	float4 colorParams = (float4) (absolute(w), m);
	float dz = 1.0f;

	for (int i = 0; i < ITERATIONS; i++)
	{
		dz = 8.0f * pow(sqrt(m), 7.0f) * dz + 1.0f;

		// Calculate power
		float r = length(w);
		float b = power * acos(w.y / r);
		float a = power * atan2(w.x, w.z);
		w = pow(r, power) * (float3) (sin(b) * sin(a), cos(b), sin(b) * cos(a)) + position;

		colorParams = min(colorParams, (float4) (absolute(w), m));
		m = dot(w, w);

		if (m > 256.0f) break;
	}

	material.ambient = (float3)(colorParams.x, colorParams.y, colorParams.z);
	material.diffuse = material.ambient;
	material.specular = (float3)(0.5f, 0.5f, 0.5f);
	material.shininess = 50.0f;

	// Distance estimation
	*distance = 0.25f * log(m) * sqrt(m) / dz;

	return material;
}

float boundingVolumeDE(float3 position, float time)
{
	return sphereSDF(position, (float3)(0, 0, 0), 1.25f);
}

#endif

Material getMaterial(float3 position, float time)
{
	float distance;
	return mandelbulbSDF(position, time, &distance);
}



float DE(float3 position, float time)
{
	float distance;
	mandelbulbSDF(position, time, &distance);
	return distance;
}

#include "main.cl"
