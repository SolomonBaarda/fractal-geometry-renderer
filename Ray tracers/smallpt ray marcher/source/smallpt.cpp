// https://www.kevinbeason.com/smallpt/

#include <math.h>   // smallpt, a Path Tracer by Kevin Beason, 2008
#include <stdlib.h> // Make : g++ -O3 -fopenmp smallpt.cpp -o smallpt
#include <stdio.h>  //        Remove "-fopenmp" for g++ version < 4.2
#include <random>
#include <iostream>

#include "structs.h"
#include "signedDistanceFunctions.h"

#ifndef M_PI
#define M_PI 3.14159265359
#endif

#ifndef M_1_PI
#define M_1_PI (1.0 / M_PI)
#endif





float signedDistanceEstimation(const Vector3& point)
{
	const float objects[] = {
		sphereSDF(point, Vector3(0, 0, -1), 0.5f)
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

		if (distanceEstimation < minimumDistancePerIteration) break;
	}

	float c = 1.0 - (float(steps) / float(maximumRaySteps));

	return Vector3(c, c, c);
}



inline double erand48()
{
	return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
}

inline double clamp(double x)
{
	return x < 0 ? 0 : x > 1 ? 1 : x;
}

inline int32_t toInt(double x)
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
		fprintf(stderr, "\rRendering %5.2f%%", 100.0f * static_cast<float>(y) / (static_cast<float>(height) - 1));

		// Columns
		for (int32_t x = 0; x < width; x++)
		{
			float u = float(x) / (width - 1);
			float v = float(y) / (height - 1);

			Vector3 rayDirection = (lower_left_corner + horizontal * u + vertical * v - origin).normalised();
			image[y * width + x] = trace(origin, rayDirection, 100, 0.001f);
		}
	}

	saveImageToFile(image, width, height, "image.ppm");

	return 0;
}

