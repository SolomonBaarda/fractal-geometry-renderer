#include "utils.cl"
#include "types.cl"
#include "sdf.cl"

#define CAMERA_POSITIONS_LENGTH 1
#define CAMERA_POSITIONS_ARRAY { (float4)(5, 0, 0, 0) }

#define CAMERA_FACING_DIRECTIONS_LENGTH 1
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(-1, 0, 0)), 0) }

#define CAMERA_SPEED 2.5f

#define SCENE_BACKGROUND_COLOUR (float3)(0.3f, 0.3f, 0.3f)
#define DO_SOFT_SHADOWS true

#define MAXIMUM_MARCH_STEPS 256
#define MAXIMUM_MARCH_DISTANCE 15.0f
#define SURFACE_INTERSECTION_EPSILON 0.0001f

#define DO_RENDER_SURFACE_NORMALS false
#define DO_RENDER_MARCHING_ITERATIONS false
#define DISPLAY_BOUNDING_VOLUME false

Light getLight(float time)
{
	Light light;
	light.ambient = (float3)(0.1f, 0.1f, 0.1f);
	light.diffuse = (float3)(0.6f, 0.6f, 0.6f);
	light.specular = (float3)(1.0f, 1.0f, 1.0f);
	light.position = (float3)(5.0f, -5.0f, 5.0f);

	return light;
}

float len(const float2 vec)
{
	return sqrt(vec.x * vec.x + vec.y * vec.y);
}

Material SDF(const float3 position, const float time, float* distance)
{
	// https://www.shadertoy.com/view/4ltSW8

	float3 p = position;

	// thickness
	float t = 0.02f;
	float d = 1e10;

	//p.y += .5;
	//p.xy *= rot(PI / 2.);

	float3  q = p + (float3)(1.0f - cos((1.0f - p.y) / 3.0f * PI), 0.0f, 0.0f);
	float y = pow(sin((1. - p.y) / 3. * PI / 2.), 2.);

	float tube_hollow = max(max(f_abs(len((float2)(q.x, q.z)) - 0.5f + 0.25f * y) - t, q.y - 1.0f), -q.y - 2.0f);
	float tube_solid = max(max(len((float2)(q.x, q.z)) - 0.5f + 0.25f * y, q.y - 1.0f), -q.y - 2.0f);

	// opening (half XZ torus)
	q = p - (float3)(0.0f, 1.0f, 0.0f);
	d = min(d, max(f_abs(len((float2)(len((float2)(q.x, q.z)) - 1.0f, q.y)) - 0.5f) - t, -q.y));

	// body (stretched XZ torus)
	q = p;
	d = min(d, max(max(max(f_abs(len((float2)(q.x, q.z)) - 1.5f + 1.25f * y), q.y - 1.0f), -q.y - 2.0f) - t, -tube_solid));

	// tube (stretched XZ cylinder)
	d = min(d, tube_hollow);

	// handle (half XY torus)
	q = p + (float3)(1.0f, 2.0f, 0.0f);
	d = min(d, max(f_abs(len((float2)(len((float2)(q.x, q.y)) - 1.0f, q.z)) - 0.25f) - t, q.y));

	// Distance estimation
	*distance = d;

	// Material
	Material material;
	float time_scaled = time * 0.5f;
	material.ambient = ((float3)(sin(time_scaled), sin(time_scaled + 0.666f), sin(time_scaled + 1.333f)) + 1) / 2.0f;
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

float DE(float3 position, float time)
{
	float distance;
	SDF(position, time, &distance);
	return distance;
}

#include "main.cl"
