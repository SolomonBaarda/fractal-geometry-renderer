#include "utils.cl"

#define CAMERA_POSITIONS_LENGTH 3
#define CAMERA_POSITIONS_ARRAY { (float4)(-4.0, -1.5, 0.0, 5), (float4)(-2.0, -1.4, 0.0, 10), (float4)(-0.6, -1.5, 0.0, 20) }

#define CAMERA_FACING_DIRECTIONS_LENGTH 3
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(-0.7, -0.25, 0.0)), 5), (float4)(normalise((float3)(-0.7, -0.3, 0.0)), 10), (float4)(normalise((float3)(-0.7, -0.6, 0.0)), 20) }

#define DO_BENCHMARK
#define BENCHMARK_START_STOP_TIME (float2)(1.0f, 40.0f)
#define CAMERA_DO_LOOP false

#define MAXIMUM_MARCH_STEPS 100
#define MAXIMUM_MARCH_DISTANCE 50.0f

#define SURFACE_INTERSECTION_EPSILON 0.001f

#define CAMERA_FOCUS_DISTANCE 0.001f

#define DO_GEOMETRY_GLOW true

#define SCENE_GLOW_COLOUR (float3)(0.8f, 0.8f, 0.8f)
#define SCENE_BACKGROUND_COLOUR (float3)(0.1f, 0.1f, 0.1f)

#define SCENE_MAX_GLOW_DISTANCE 0.1f

//#define FORCE_FREE_CAMERA
#define CAMERA_SPEED 0.5f

#define DO_HARD_SHADOWS true



#define ITERATIONS 10

#include "types.cl"

Light getLight(float time)
{
	Light light;
	light.ambient = (float3)(0.1f, 0.1f, 0.1f);
	light.diffuse = (float3)(0.5f, 0.5f, 0.5f);
	light.specular = (float3)(1.0f, 1.0f, 1.0f);
	light.position = (float3)(0.0, -5, -5);

	return light;
}

Material getMaterial(float3 position, float time)
{
	Material material;

	float power = 7.75f + time * 0.01f;

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

	return material;
}

float signedDistanceEstimation(float3 position, float time)
{
	float power = 7.75f + time * 0.01f;

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

	return 0.25f * log(m) * sqrt(m) / dz;
}

#include "main.cl"
