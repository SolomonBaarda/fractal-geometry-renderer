#pragma once

#include <cmath>

#include "structs.h"

static float max(float a, float b)
{
	return a > b ? a : b;
}

static float min(float a, float b)
{
	return a < b ? a : b;
}

// https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm

static float sphereSDF(const Vector3& point, const Vector3& sphereCentre, float sphereRadius)
{
	Vector3 relativePosition = sphereCentre - point;
	return relativePosition.length() - sphereRadius;
}

//float static boxSDF(const Vector3& point, const Vector3& boxCentre, const Vector3& boxDimensions)
//{
//	Vector3 relativePosition = boxCentre - point;
//	Vector3 q = relativePosition.absolute() - (boxDimensions / 2);
//	return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
//}

//float static planeSDF(const Vector3& point, const Vector3& planeCentre, const Vector3& planeDimensions)
//{
//	Vector3 relativePosition = planeCentre - point;
//	return dot(relativePosition, planeDimensions) + h;
//}


float signedDistanceEstimation(const Vector3& point, Vector3& outputColour = Vector3())
{
	int length = 3;
	const float objects[] = {
		sphereSDF(point, Vector3(0, 0, -1), 0.5f),
		sphereSDF(point, Vector3(1, 0.5f, -1), 0.25f),
		sphereSDF(point, Vector3(-1, -0.5f, -1), 0.25f)
	};
	const Vector3 colours[] =
	{
		Vector3(1.0f, 0, 0.25f),
		Vector3(0, 1.0f, 0.25f),
		Vector3(0, 0.25f, 1.0f)
	};

	float min = objects[0];
	outputColour = colours[0];

	for (int i = 0; i < length; i++)
	{
		if (objects[i] < min)
		{
			min = objects[i];
			outputColour = colours[i];
		}
	}

	return min;
}

Vector3 estimateSurfaceNormal(const Vector3& surface)
{
	const float epsilon = 0.01f;

	const Vector3 xOffset = Vector3(epsilon, 0, 0), yOffset = Vector3(0, epsilon, 0), zOffset = Vector3(0, 0, epsilon);

	float x = signedDistanceEstimation(surface + xOffset) - signedDistanceEstimation(surface - xOffset);
	float y = signedDistanceEstimation(surface + yOffset) - signedDistanceEstimation(surface - yOffset);
	float z = signedDistanceEstimation(surface + zOffset) - signedDistanceEstimation(surface - zOffset);

	return Vector3(x, y, z).normalised();
}

inline float clamp01(float x)
{
	return x < 0 ? 0 : x > 1 ? 1 : x;
}

Vector3 phong(const Vector3& n, const Vector3& v)
{
	//material parameters
	const float ks = 3.0;
	const float kd = 3.0;
	const float ka = 1.0;
	const float al = 20.0;
	//light parameters
	const float ia = 1.0;
	const Vector3 lm = Vector3(5, 3, -1).normalised();
	const float id = 1.0;
	const float is = 1.0;



	Vector3 rm = n * 2.0 * Vector3::dotProduct(lm, n) - lm;

	float ip = ka * ia + (kd * clamp01(Vector3::dotProduct(lm, n)) * id + ks * pow(clamp01(Vector3::dotProduct(rm, v)), al) * is);

	return Vector3(0.1, 0.2, 0.5) * ip;

}

Vector3 render(const Vector3& position, const Vector3& direction)
{
	const int maximumRaySteps = 100;
	const float surfaceCollisionThreshold = 0.000001f;


	float totalDistance = 0.0;

	for (int steps = 0; steps < maximumRaySteps; steps++)
	{
		Vector3 currentPosition = position + direction * totalDistance;
		Vector3 colour;

		float distanceEstimation = signedDistanceEstimation(currentPosition, colour);
		totalDistance += distanceEstimation;

		if (distanceEstimation <= surfaceCollisionThreshold)
		{
			Vector3 normal = estimateSurfaceNormal(currentPosition);

			// Render normals
			//colour = (normal + Vector3(1, 1, 1)) * 0.5f;

			colour = colour * phong(normal, direction);

			return colour;
		}
	}

	return Vector3();
}