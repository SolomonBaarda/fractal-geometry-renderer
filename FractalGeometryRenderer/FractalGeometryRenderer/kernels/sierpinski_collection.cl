#ifndef BENCHMARK

#define CAMERA_POSITIONS_LENGTH 1
#define CAMERA_POSITIONS_ARRAY {  (float4)(10, 0, 0, 0)}
#define CAMERA_FACING_DIRECTIONS_LENGTH 1
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(1, 0, 0)), 0) }
#define FORCE_FREE_CAMERA true

//#define DO_BOUNDING_VOLUME_OPTIMISATION true
//#define DO_SOFT_SHADOWS true
//#define DO_HARD_SHADOWS true

//#define DO_GEOMETRY_GLOW true

#endif

#define MAXIMUM_MARCH_STEPS 200
#define MAXIMUM_MARCH_DISTANCE 50.0f
#define SURFACE_INTERSECTION_EPSILON 0.0001f
#define CAMERA_SPEED 1.0f
#define SCENE_BACKGROUND_COLOUR (float3)(0.78f, 0.78f, 0.73f)
#define SCENE_GLOW_COLOUR (float3)(0.8f, 0.8f, 0.8f)
#define SCENE_MAX_GLOW_DISTANCE 0.1f
#define CAMERA_FOCUS_DISTANCE 0.01f
#define SURFACE_SHADOW_FALLOFF 5.0f


#include "types.cl"
#include "sdf.cl"
#include "sierpinski.cl"

Light getLight(float time)
{
	Light light;
	light.ambient = (float3)(0.1f, 0.1f, 0.1f);
	light.diffuse = (float3)(0.5f, 0.5f, 0.5f);
	light.specular = (float3)(1.0f, 1.0f, 1.0f);
	light.position = (float3)(5, -5, 5);

	return light;
}

Material SDF(const float3 position, const float time, float* distance)
{
	float cubeDistance = 0.0f;
	float tetrahedronDistance = 0.0f;

	Material cube = sierpinskiCubeSDF((float3)(1, 1, 1) - position, 7, &cubeDistance);
	Material tetrahedron = sierpinskiTetrahedronSDF((float3)(-5, 0, 0) - position, 12, 100000000, &tetrahedronDistance);

	if (cubeDistance < tetrahedronDistance)
	{
		*distance = cubeDistance;
		return cube;
	}
	else
	{
		*distance = tetrahedronDistance;
		return tetrahedron;
	}
}

Material getMaterial(float3 position, float time)
{
	float distance;
	return SDF(position, time, &distance);
}

float boundingVolumeDE(float3 position, float time)
{
	return min(boxSDF(position, (float3)(0, 0, 0), 1.0f), sphereSDF(position, (float3)(-5, 0, 0), 1.75f));
}

float DE(float3 position, float time)
{
	float distance;
	SDF(position, time, &distance);
	return distance;
}

#include "main.cl"
