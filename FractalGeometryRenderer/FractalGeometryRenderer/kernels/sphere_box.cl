#define CAMERA_POSITIONS_LENGTH 1
#define CAMERA_POSITIONS_ARRAY { (float4)(10, -10, -10, 0) }

#define CAMERA_FACING_DIRECTIONS_LENGTH 1
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(-10, -10, -10)), 0.0f) }

#define SCENE_BACKGROUND_COLOUR (float3)(0.78f, 0.78f, 0.73f)

#define SURFACE_INTERSECTION_EPSILON 0.00001f
#define MAXIMUM_MARCH_STEPS 200
#define MAXIMUM_MARCH_DISTANCE 50.0f

//#define DO_SOFT_SHADOWS true
#define DO_HARD_SHADOWS true

#include "types.cl"
#include "sdf.cl"

Light getLight(float time)
{
	Light light;
	light.ambient = (float3)(0.2f, 0.2f, 0.2f);
	light.diffuse = (float3)(0.5f, 0.5f, 0.5f);
	light.specular = (float3)(1.0f, 1.0f, 1.0f);
	light.position = (float3)(25, -25, 25);

	return light;
}

Material getMaterial(float3 position, float time)
{
	// Material
	Material material;
	material.ambient = (float3)(0.92f, 0.30f, 0.16f);
	material.diffuse = material.ambient;
	material.specular = (float3)(0.5f, 0.5f, 0.5f);
	material.shininess = 25.0f;

	return material;
}

float DE(float3 position, float time)
{
	float offset = -sin(time * 0.5f) * 1.0f;

	float distance = opSmoothUnion(
		sphereSDF(position, (float3)(0.0f, offset, 0.0f), 3.5f),
		boxSDF(position, (float3)(0.0f, offset, 0.0f), (float3)(4.0f, 0.5f, 4.0f)),
		(-sin(time * 0.5f) + 1.0f) * 1.5f
	);

	return distance;
}

#include "main.cl"
