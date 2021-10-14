#pragma once

#include <cstdint>
#include "Vector3.h"

// https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm

float static sphereSDF(const Vector3& point, const Vector3& sphereCentre, float sphereRadius)
{
	Vector3 relativePosition = sphereCentre - point;
	return (relativePosition).magnitude() - sphereRadius;
}



float static boxSDF(const Vector3& point, const Vector3& boxCentre, const Vector3& boxDimensions)
{
	Vector3 relativePosition = boxCentre - point;
	Vector3 q = relativePosition.absolute() - (boxDimensions / 2);
	return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
}

float static planeSDF(const Vector3& point, const Vector3& planeCentre, const Vector3& planeDimensions)
{
	Vector3 relativePosition = planeCentre - point;
	return dot(relativePosition, planeDimensions) + h;
}


float signedDistanceEstimation(const Vector3& point)
{
	const float objects[] = {
		boxSDF(point, Vector3(0, 0, 0), Vector3(30, 20, 10)),
		sphereSDF(point, Vector3(-50, 20, 0), 30),
		sphereSDF(point, Vector3(60, -40, 0), 40),
		sphereSDF(point, Vector3(10, 50, 0), 30),
	};

	float min = objects[0];

	for (float f : objects)
	{
		if (f < min)
			min = f;
	}

	return min;

}



Vector3 trace(const Vector3& origin, const Vector3& direction, const uint32_t maximumRaySteps, const uint32_t minimumDistancePerIteration)
{
	// http://blog.hvidtfeldts.net/index.php/2011/06/distance-estimated-3d-fractals-part-i/

	float totalDistance = 0.0;
	int steps;

	for (steps = 0; steps < maximumRaySteps; steps++)
	{
		Vector3 position = origin + (direction * totalDistance);
		float distanceEstimation = signedDistanceEstimation(position);
		totalDistance += distanceEstimation;

		if (distanceEstimation < minimumDistancePerIteration)
		{
			//return Vector3(1, 1, 1);
			break;
		}
	}

	float c = 1.0 - (float(steps) / float(maximumRaySteps));

	return Vector3(c, c, c);
}