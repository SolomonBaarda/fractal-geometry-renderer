#include "utils.cl"
#include "types.cl"
#include "sdf.cl"

#define CAMERA_POSITIONS_LENGTH 1
#define CAMERA_POSITIONS_ARRAY { (float4)(5, 0, 0, 0) }

#define CAMERA_FACING_DIRECTIONS_LENGTH 1
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(-1, 0, 0)), 0) }

#define CAMERA_SPEED 2.5f

#define SCENE_BACKGROUND_COLOUR (float3)(0.1f, 0.1f, 0.1f)
#define DO_SOFT_SHADOWS true

#define MAXIMUM_MARCH_STEPS 100
#define MAXIMUM_MARCH_DISTANCE 50.0f

Light getLight(float time)
{
	Light light;
	light.ambient = (float3)(0.2f, 0.2f, 0.2f);
	light.diffuse = (float3)(0.5f, 0.5f, 0.5f);
	light.specular = (float3)(1.0f, 1.0f, 1.0f);
	light.position = (float3)(0.0, -4, -4);

	return light;
}

Material SDF(const float3 position, const float time, float* distance)
{
	// Distance estimation
	*distance = sphereSDF(position, (float3)(0, 0, 0), 1.0f);

	// Material
	Material material;
	material.ambient = (float3)(0.75f, 0.25f, 0.5f);
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
