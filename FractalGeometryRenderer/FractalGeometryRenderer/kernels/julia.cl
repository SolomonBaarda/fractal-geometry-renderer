#include "utils.cl"

#ifndef BENCHMARK

// Config
#define CAMERA_POSITIONS_LENGTH 1
#define CAMERA_POSITIONS_ARRAY { (float4)(5, -5, 5, 0) }
#define CAMERA_FACING_DIRECTIONS_LENGTH 1
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(1, 1, 1)), 0) }
#define FORCE_FREE_CAMERA true

// Features
//#define DO_GEOMETRY_GLOW true
//#define DO_SOFT_SHADOWS true
//#define DO_HARD_SHADOWS false
//#define USE_BOUNDING_VOLUME true
//#define DISPLAY_BOUNDING_VOLUME false

#endif

// Scene
#define MAXIMUM_MARCH_STEPS 100
#define MAXIMUM_MARCH_DISTANCE 25.0f
#define SURFACE_INTERSECTION_EPSILON 0.001f
#define SCENE_GLOW_COLOUR (float3)(0.8f, 0.8f, 0.8f)
#define SCENE_MAX_GLOW_DISTANCE 0.05f
#define SCENE_BACKGROUND_COLOUR (float3)(0.1f, 0.1f, 0.1f)
#define CAMERA_SPEED 0.5f
#define SURFACE_SHADOW_EPSILON 0.01f
#define SURFACE_SHADOW_FALLOFF 10.0f

#define ITERATIONS 10

#include "types.cl"
#include "sdf.cl"
#include "complex.cl"


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


// a + ib

float calcDistance(Complex z0, Complex c)
{
	Complex z = z0;
	float d2 = 1.0f;
	float m2 = 0.0f;
	for (int i = 0; i < 1024; i++)
	{
		d2 *= 4.0f * m2;
		z = add(multiply(z, z), c);
		float temp = fmod(z.real, z.imaginary);
		m2 = temp * temp;

		if (m2 > 1e20) break;
	}

	return sqrt(m2 / d2) * 0.5f * log(m2);
}


Material SDF(const float3 position, const float time, float* distance)
{
	// Material
	Material material;

	Complex a;
	a.real = position.x;
	a.imaginary = position.z;

	Complex b;
	b.real = 4.0f;
	b.imaginary = 3.0f;

	// Distance estimation
	*distance = calcDistance(a, b);

	material.ambient = (float3)(0.5);
	material.diffuse = material.ambient;
	material.specular = (float3)(0.5f, 0.5f, 0.5f);
	material.shininess = 50.0f;

	return material;
}

Material getMaterial(float3 position, float time)
{
	float distance;
	return SDF(position, time, &distance);
}

float boundingVolumeDE(float3 position, float time)
{
	return sphereSDF(position, (float3)(0, 0, 0), 1.25f);
}

float DE(float3 position, float time)
{
	float distance;
	SDF(position, time, &distance);
	return distance;
}

#include "main.cl"
