//#include "Vector3.h"
//#include "scene.cl"

#ifndef __OPENCL_C_VERSION__
#include <cmath>
#endif

// https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm

//float min(float a, float b)
//{
//	if (a < b)
//		return a;
//	else
//		return b;
//}

float magnitude(float3 vec)
{
	return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

float sphereSDF(float3 position, float3 sphereCentre, float sphereRadius)
{
	float3 relative = sphereCentre - position;
	return magnitude(relative) - sphereRadius;
}

// return colour, distance
float4 signedDistanceEstimation(float3 position)
{
	// 	const Vector3 colours[] =
	//{
	//	Vector3(1.0f, 0, 0.25f),
	//		Vector3(0, 1.0f, 0.25f),
	//		Vector3(0, 0.25f, 1.0f)
	//};

	float3 objects = (float3)(
		sphereSDF(position, (float3)(0, 0, -1), 0.5f),
		sphereSDF(position, (float3)(1, 0.5f, -1), 0.25f),
		sphereSDF(position, (float3)(-1, -0.5f, -1), 0.25f)
		);

	// colour.x, colour.y, colour.z, distance
	return (float4) ((float3)(0, 0, 0), min(objects.x, min(objects.y, objects.z)));
}

float3 normalise(float3 vec)
{
	return vec * (1 / magnitude(vec));
}

float3 getSurfaceNormal(float3 position)
{
	float d = signedDistanceEstimation(position).w;

	return normalise((float3) (
		d - signedDistanceEstimation(position - (float3)(0.01f, 0, 0)).w,
		d - signedDistanceEstimation(position - (float3)(0, 0.01f, 0)).w,
		d - signedDistanceEstimation(position - (float3)(0, 0, 0.01f)).w
		));
}

float4 rayMarch(float3 position, float3 direction, int maximumRaySteps, float minimumDistancePerIteration, float3* surfacePosition)
{
	// http://blog.hvidtfeldts.net/index.php/2011/06/distance-estimated-3d-fractals-part-i/

	float totalDistance = 0.0;

	for (int steps = 0; steps < maximumRaySteps; steps++)
	{
		float3 curentPosition = position + direction * totalDistance;

		float4 colourAndDistance = signedDistanceEstimation(curentPosition);
		totalDistance += colourAndDistance.w;

		if (colourAndDistance.w < minimumDistancePerIteration)
		{
			surfacePosition = &curentPosition;
			return (float4)(colourAndDistance.xyz, totalDistance);
		}
	}

	return (float4)(0, 0, 0, totalDistance);
}

__kernel void getColourForPixel(__global const float3* position, __global const float3* direction, __global float3* colour)
{
	// http://blog.hvidtfeldts.net/index.php/2011/06/distance-estimated-3d-fractals-part-i/

	int ID = get_global_id(0);


	float totalDistance = 0.0;
	float3 surfacePosition;

	float4 colourAndDistance = rayMarch(*position, *direction, 100, 0.001f, &surfacePosition);
	float3 normal = getSurfaceNormal(surfacePosition);

	colour[ID] = (normal + (float3)(1, 1, 1)) * 0.5f;
	//colour[ID] = colourAndDistance.xyz;
}
