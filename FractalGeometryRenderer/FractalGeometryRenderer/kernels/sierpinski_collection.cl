#ifndef BENCHMARK

#define CAMERA_POSITIONS_LENGTH 1
#define CAMERA_POSITIONS_ARRAY {  (float4)(0, 0, 0, 0)}
#define CAMERA_FACING_DIRECTIONS_LENGTH 1
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(1, 0, 0)), 0) }
#define FORCE_FREE_CAMERA true

#define DO_BOUNDING_VOLUME_OPTIMISATION true
#define DO_SOFT_SHADOWS true

#endif

#define MAXIMUM_MARCH_STEPS 200
#define MAXIMUM_MARCH_DISTANCE 20.0f
#define SURFACE_INTERSECTION_EPSILON 0.000001f
#define CAMERA_SPEED 1.0f
#define SCENE_BACKGROUND_COLOUR (float3)(0.78f, 0.78f, 0.73f)

#include "types.cl"
#include "sdf.cl"
#include "sierpinski.cl"

Light getLight(float time)
{
	Light light;
	light.ambient = (float3)(0.1f, 0.1f, 0.1f);
	light.diffuse = (float3)(0.5f, 0.5f, 0.5f);
	light.specular = (float3)(1.0f, 1.0f, 1.0f);
	light.position = (float3)(0, 0, 0);

	return light;
}

Material SDF(const float3 position, const float time, float* distance)
{
	float cubeDistance = 0.0f;
	float tetrahedronDistance = 0.0f;
	Material cube = sierpinskiCubeSDF((float3)(-2, 0, -2) - position, 7, &cubeDistance);
	Material tetrahedron = sierpinskiTetrahedronSDF((float3)(2, 0, 2) - position, 15, 100000000, &tetrahedronDistance);

	if (cubeDistance <= tetrahedronDistance)
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
	return min(boxSDF(position, (float3)(-1, -1, -1), 1.0f), sphereSDF(position, (float3)(2, 0, 2), 1.75f));
}

float DE(float3 position, float time)
{
	float distance;
	SDF(position, time, &distance);
	return distance;
}

#include "main.cl"
