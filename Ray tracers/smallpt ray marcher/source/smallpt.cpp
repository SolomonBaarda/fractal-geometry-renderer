// https://www.kevinbeason.com/smallpt/

#include <math.h>   // smallpt, a Path Tracer by Kevin Beason, 2008
#include <stdlib.h> // Make : g++ -O3 -fopenmp smallpt.cpp -o smallpt
#include <stdio.h>  //        Remove "-fopenmp" for g++ version < 4.2
#include <random>
#include <iostream>

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

bool rayMarch(const Vector3& position, const Vector3& direction, const uint32_t maximumRaySteps, const uint32_t minimumDistancePerIteration, float& totalDistance, Vector3& outputColour = Vector3(), Vector3& surfacePosition = Vector3())
{
	// http://blog.hvidtfeldts.net/index.php/2011/06/distance-estimated-3d-fractals-part-i/

	totalDistance = 0.0;

	for (int steps = 0; steps < maximumRaySteps; steps++)
	{
		Vector3 curentPosition = position + direction * totalDistance;

		Vector3 colour;

		float distanceEstimation = signedDistanceEstimation(curentPosition, colour);
		totalDistance += distanceEstimation;

		if (distanceEstimation < minimumDistancePerIteration)
		{
			outputColour = colour;
			surfacePosition = curentPosition;
			return true;
		}
	}

	return false;
}

Vector3 GetSurfaceNormal(const Vector3& surfacePosition, const Vector3& probeDistance)
{
	float d = signedDistanceEstimation(surfacePosition);

	return Vector3(
		d - signedDistanceEstimation(surfacePosition - Vector3(0.01f, 0, 0)),
		d - signedDistanceEstimation(surfacePosition - Vector3(0, 0.01f, 0)),
		d - signedDistanceEstimation(surfacePosition - Vector3(0, 0, 0.01f))).normalised();
}

Vector3 getPixelColour(const Vector3& position, const Vector3& direction)
{
	float distance;
	Vector3 colour, surface;
	bool hitObject = rayMarch(position, direction, 100, 0.001f, distance, colour, surface);

	if (hitObject)
	{
		Vector3 normal = GetSurfaceNormal(surface, Vector3());
		return (normal + Vector3(1, 1, 1)) * 0.5f;
	}
	else
	{
		return Vector3();
	}
}



inline float clamp(float x)
{
	return x < 0 ? 0 : x > 1 ? 1 : x;
}

inline int32_t toInt(float x)
{
	// Applies a gamma correction of 2.2
	return static_cast<int32_t>(pow(clamp(x), 1 / 2.2) * 255 + .5);
}

void saveImageToFile(Vector3* image, const int32_t width, const int32_t height, const char* filename)
{
	FILE* f = fopen(filename, "w"); // Write image to PPM file.
	fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);

	// Rows
	for (int32_t y = 0; y < height; y++)
	{		// Columns
		for (int32_t x = 0; x < width; x++)
		{
			int32_t index = y * width + x;
			fprintf(f, "%d %d %d ", toInt(image[index].x), toInt(image[index].y), toInt(image[index].z));
		}

	}

	fclose(f);
}

int main(int argc, char* argv[])
{
	const float aspect_ratio = 16.0f / 9.0f;

	// Image
	int32_t width = 1024, height = static_cast<int>(width / aspect_ratio);
	int32_t totalSamplesPerPixel = 10;

	Vector3* image = new Vector3[static_cast<int64_t>(width) * static_cast<int64_t>(height)];

	// Camera
	float viewport_height = 2.0f;
	float viewport_width = aspect_ratio * viewport_height;
	float focal_length = 1.0f;

	Vector3 origin(0, 0, 0);
	Vector3 horizontal(viewport_width, 0, 0);
	Vector3 vertical(0, viewport_height, 0);
	Vector3 lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vector3(0, 0, focal_length);


#pragma omp parallel for schedule(dynamic, 1)  // OpenMP

	// Rows
	for (int32_t y = 0; y < height; y++)
	{
		fprintf(stderr, "\rRendering %5.2f%%", 100.0f * static_cast<float>(y) / static_cast<float>(height - 1));

		// Columns
		for (int32_t x = 0; x < width; x++)
		{
			float u = static_cast<float>(x) / (width - 1);
			float v = static_cast<float>(y) / (height - 1);

			Vector3 rayDirection = (lower_left_corner + horizontal * u + vertical * v - origin);

			// Ray march from the screen position in the ray direction
			image[y * width + x] = getPixelColour(origin + rayDirection, rayDirection.normalised());
		}
	}

	saveImageToFile(image, width, height, "image.ppm");

	return 0;
}

