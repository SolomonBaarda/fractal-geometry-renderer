#include "utils.cl"
#include "types.cl"
#include "sdf.cl"

#ifndef BENCHMARK

#define CAMERA_POSITIONS_LENGTH 1
#define CAMERA_POSITIONS_ARRAY { (float4)(15, -5, 0, 0) }
#define CAMERA_FACING_DIRECTIONS_LENGTH 1
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(1, 0, 0)), 0.0f) }

#define DO_SOFT_SHADOWS true

#endif

#define SCENE_BACKGROUND_COLOUR (float3)(0.1f, 0.1f, 0.1f)

#define SURFACE_INTERSECTION_EPSILON 0.00001f
#define MAXIMUM_MARCH_STEPS 300
#define MAXIMUM_MARCH_DISTANCE 100
#define SURFACE_SHADOW_FALLOFF 5.0f

Light getLight(float time)
{
	float t = fmod(time * 0.75f, 2.0f * PI);

	Light light;
	light.ambient = (float3)(0.1f, 0.1f, 0.1f);
	light.diffuse = (float3)(0.6f, 0.6f, 0.6f);
	light.specular = (float3)(1.0f, 1.0f, 1.0f);
	light.position = (float3)(25 * cos(t), -25, 25 * sin(t));

	return light;
}

Material SDF(float3 position, float time, float * distance)
{
	float offset = -sin(time * 0.5f) * 1.5f;
	float dist_sphere = sphereSDF(position, (float3)(0.0f, offset - 5.0f, 0.0f), 1.0f);
	float dist_plane = f_abs(position.y);

	Material material;

	if (dist_sphere < dist_plane)
	{
		*distance = dist_sphere;

		material.ambient = (float3)(0.9f, 0.9f, 0.9f);
		material.diffuse = material.ambient;
		material.specular = (float3)(0.5f, 0.5f, 0.5f);
		material.shininess = 25.0f;
	}
	else
	{
		*distance = dist_plane;

		material.ambient = (float3)(0.92f, 0.30f, 0.16f);
		material.diffuse = material.ambient;
		material.specular = (float3)(0.0f, 0.0f, 0.0f);
		material.shininess = 100.0f;
	}

	return material;
}

Material getMaterial(float3 position, float time)
{
	float distance;
	return SDF(position, time, &distance);
}

float DE(float3 position, float time)
{
	float distance;
	SDF(position, time, &distance);
	return distance;
}

#include "main.cl"
