#define CAMERA_POSITIONS_LENGTH 1
#define CAMERA_POSITIONS_ARRAY { (float4)(5, 1, 1, 0) }

#define CAMERA_FACING_DIRECTIONS_LENGTH 1
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(-0.5, -0.5, -0.5)), 0.0f) }

#define MAXIMUM_MARCH_STEPS 100
#define MAXIMUM_MARCH_DISTANCE 10.0f

#define SURFACE_INTERSECTION_EPSILON 0.000001f

#define FORCE_FREE_CAMERA true
#define CAMERA_SPEED 1.0f

#define USE_BOUNDING_VOLUME true


// Debug
#define DO_RENDER_SURFACE_NORMALS false
#define DO_RENDER_MARCHING_ITERATIONS false
#define DISPLAY_BOUNDING_VOLUME false




#include "sierpinski.cl"

#include "types.cl"
#include "sdf.cl"

Light getLight(float time)
{
	Light light;
	light.ambient = (float3)(0.2f, 0.2f, 0.2f);
	light.diffuse = (float3)(0.5f, 0.5f, 0.5f);
	light.specular = (float3)(1.0f, 1.0f, 1.0f);
	light.position = (float3)(0.0, -4, -4);

	return light;
}

Material getMaterial(float3 position, float time)
{
	float distance;
	return sierpinskiCubeSDF(position, 10, &distance);
}

float DE(float3 position, float time)
{
	float distance;
	sierpinskiCubeSDF(position, 8, &distance);
	return distance;
}

float boundingVolumeDE(float3 position, float time)
{
	return boxSDF(position, (float3)(1, 1, 1), (float3)(1, 1, 1));
}

#include "main.cl"
