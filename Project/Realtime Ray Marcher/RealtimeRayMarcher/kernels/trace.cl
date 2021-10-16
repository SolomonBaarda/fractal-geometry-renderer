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


float sphereSDF(float3 position, float3 sphereCentre, float sphereRadius)
{
	float3 relative = sphereCentre - position;
	return sqrt(relative.x * relative.x + relative.y * relative.y + relative.z * relative.z) - sphereRadius;
	//return (relativePosition).magnitude() - sphereRadius;
}


float signedDistanceEstimation(float3 position)
{
	float3 objects = (float3)(
		sphereSDF(position, (float3)(-50.0f, 20.0f, 0.0f), 30.0f),
		sphereSDF(position, (float3)(60.0f, -40.0f, 0.0f), 40.0f),
		sphereSDF(position, (float3)(10.0f, 50.0f, 0.0f), 30.0f)
		);

	return min(objects.x, min(objects.y, objects.z));
}

__kernel void trace(__global float3* origin, __global float3* direction, __global float* colour)
{
	// http://blog.hvidtfeldts.net/index.php/2011/06/distance-estimated-3d-fractals-part-i/

	int ID = get_global_id(0);

	const int maximumRaySteps = 25;
	const int minimumDistancePerIteration = 1;

	float totalDistance = 0.0;
	int steps;

	for (steps = 0; steps < maximumRaySteps; steps++)
	{
		float3 position = *origin + (*direction * totalDistance);
		float distanceEstimation = signedDistanceEstimation(position);

		totalDistance += distanceEstimation;

		if (distanceEstimation < minimumDistancePerIteration)
		{
			break;
		}
	}

	//colour[ID] = (float3)(1.0f - (float(steps) / float(maximumRaySteps)));
	colour[ID] = 1.0f - ((float)(steps) / (float)(maximumRaySteps));
}
