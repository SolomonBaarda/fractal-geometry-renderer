#define CAMERA_POSITIONS_LENGTH 1
#define CAMERA_POSITIONS_ARRAY { (float4)(-0.4, -0.6, -0.4, 0) }

#define CAMERA_FACING_DIRECTIONS_LENGTH 1
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(-0.5, -0.5, -0.5)), 0.0f) }

#define MAXIMUM_MARCH_STEPS 200
#define MAXIMUM_MARCH_DISTANCE 10.0f

#define SURFACE_INTERSECTION_EPSILON 0.000001f

#define FORCE_FREE_CAMERA true
#define CAMERA_SPEED 1.0f

#define SCENE_LIGHT_POSITION (float3)(0, 0, 0)
#define SCENE_LIGHT_COLOUR (float3)(1.0f, 1.0f, 1.0f)
#define SCENE_BACKGROUND_COLOUR (float3)(0.78f, 0.78f, 0.73f)

#define DO_LAMBERTIAN_REFLECTANCE true
#define DO_SOFT_SHADOWS true

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
	return sierpinskiTetrahedronSDF(position, 15, 100000000, &distance);
}

float DE(float3 position, float time)
{
	float distance;
	sierpinskiTetrahedronSDF(position, 15, 100000000, &distance);
	return distance;
}

#include "main.cl"
