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
	int32_t width = 1024, height = 768;
	int32_t totalSamplesPerPixel = 10;

	Ray cam(Vector3(0, 0, -300), Vector3(0, 0, 1).normalise());        // cam pos, dir

	Vector3 xDirectionIncrement = Vector3(width * .5135 / height), yDirectionIncrement = (xDirectionIncrement % cam.direction).normalise() * .5135;
	Vector3* image = new Vector3[static_cast<int64_t>(width) * static_cast<int64_t>(height)];

#pragma omp parallel for schedule(dynamic, 1)  // OpenMP

	// Rows
	for (int32_t y = 0; y < height; y++)
	{
		fprintf(stderr, "\rRendering (%d spp) %5.2f%%", totalSamplesPerPixel * 4, 100. * y / (static_cast<int64_t>(height) - 1));

		// Columns
		for (int32_t x = 0; x < width; x++)
		{
			Vector3 r;

			for (int32_t sample = 0; sample < totalSamplesPerPixel; sample++)
			{
				double r1 = 2 * erand48(), dx = r1 < 1 ? sqrt(r1) - 1 : 1 - sqrt(2 - r1);
				double r2 = 2 * erand48(), dy = r2 < 1 ? sqrt(r2) - 1 : 1 - sqrt(2 - r2);
				Vector3 rayDirection = xDirectionIncrement * (((.5 + dx) / 2 + x) / width - .5) + yDirectionIncrement * (((.5 + dy) / 2 + y) / height - .5) + cam.direction;

				r = r + trace(cam.origin + rayDirection * 140, rayDirection.normalise(), 25, 0.5);
			}

			image[y * width + x] = Vector3(clamp(r.x / totalSamplesPerPixel), clamp(r.y / totalSamplesPerPixel), clamp(r.z / totalSamplesPerPixel));
		}
	}

	saveImageToFile(image, width, height, "image.ppm");

	return 0;
}

