#define CAMERA_POSITIONS_LENGTH 2
#define CAMERA_POSITIONS_ARRAY { (float4)(8, 0, -2, 0),  (float4)(7, 0, -1, 10) }

#define CAMERA_FACING_DIRECTIONS_LENGTH 1
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(0.94, -0.06, -0.32)), 0.0f) }

#define MAXIMUM_MARCH_STEPS 200
#define MAXIMUM_MARCH_DISTANCE 20.0f

#define SURFACE_INTERSECTION_EPSILON 0.000001f

#define CAMERA_SPEED 1.0f
#define SCENE_BACKGROUND_COLOUR (float3)(0.78f, 0.78f, 0.73f)

#define BENCHMARK_START_STOP_TIME (float2)(0.0f, 15.0f)

#define DO_BOUNDING_VOLUME_OPTIMISATION true

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

Material DE(const float3 position, const float time, float* distance)
{
	// Material
	Material material;

#if DO_BOUNDING_VOLUME_OPTIMISATION

	float boundingSphereDistanceCube = sphereSDF(position, (float3)(-3, 0, -3), 2.5f);
	float boundingSphereDistanceTetrahedron = sphereSDF(position, (float3)(2, 0, 2), 1.75f);

	float4 colourAndDist = (float4)(100.0f);

	if (boundingSphereDistanceCube <= 0.01f)
	{
		colourAndDist = sierpinskiCubeSDF((float3)(-2, 0, -2) - position, 7);
	}
	else if (boundingSphereDistanceTetrahedron <= 0.01f)
	{
		colourAndDist = sierpinskiTetrahedronSDF((float3)(2, 0, 2) - position, 15, 100000000);
	}
	else
	{
		colourAndDist.w = min(boundingSphereDistanceCube, boundingSphereDistanceTetrahedron);
	}

	material.ambient = (float3)(colourAndDist.x, colourAndDist.y, colourAndDist.z);
	material.diffuse = material.ambient;
	material.specular = (float3)(0.5f, 0.5f, 0.5f);
	material.shininess = 50.0f;

	// Distance estimation
	*distance = colourAndDist.w;

#else

	float4 colourAndDistCube = sierpinskiCubeSDF((float3)(-2, 0, -2) - position, 7);
	float4 colourAndDistTet = sierpinskiTetrahedronSDF((float3)(2, 0, 2) - position, 15, 100000000);

	if (colourAndDistCube.w < colourAndDistTet.w)
	{
		material.ambient = (float3)(colourAndDistCube.x, colourAndDistCube.y, colourAndDistCube.z);
		// Distance estimation
		*distance = colourAndDistCube.w;
	}
	else
	{
		material.ambient = (float3)(colourAndDistTet.x, colourAndDistTet.y, colourAndDistTet.z);
		// Distance estimation
		*distance = colourAndDistTet.w;
	}

	material.diffuse = material.ambient;
	material.specular = (float3)(0.5f, 0.5f, 0.5f);
	material.shininess = 50.0f;

#endif

	return material;
}

Material getMaterial(float3 position, float time)
{
	float distance;
	return DE(position, time, &distance);
}

float signedDistanceEstimation(float3 position, float time)
{
	float distance;
	DE(position, time, &distance);
	return distance;
}

#include "main.cl"
