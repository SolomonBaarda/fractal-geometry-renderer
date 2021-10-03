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
		sphereSDF(point, Vector3(0, 0, 0), 20), 
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
			return Vector3(1, 1, 1);
			break;
		}
	}

	float c = 1.0 - float(steps) / float(maximumRaySteps);

	return Vector3(0, 0, 0);
}





inline double erand48(uint16_t xsubi[3])
{
	return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
}

//inline double erand48(uint16_t* X)
//{
//	std::default_random_engine generator(*X);
//	return distr(generator);
//}

inline double clamp(double x)
{
	return x < 0 ? 0 : x > 1 ? 1 : x;
}

inline int32_t toInt(double x)
{
	// Applies a gamma correction of 2.2
	return static_cast<int32_t>(pow(clamp(x), 1 / 2.2) * 255 + .5);
}

int main(int argc, char* argv[])
{
	int32_t width = 1024, height = 768;
	int32_t samps = 4;

	Ray cam(Vector3(0, 0, 300), Vector3(0, 0, -1).normalise());        // cam pos, dir

	Vector3 xDirectionIncrement = Vector3(width * .5135 / height), yDirectionIncrement = (xDirectionIncrement % cam.direction).normalise() * .5135;
	Vector3* image = new Vector3[static_cast<int64_t>(width) * static_cast<int64_t>(height)];

	Vector3 r;

#pragma omp parallel for schedule(dynamic, 1) private(r) // OpenMP

	// Loop over image rows
	for (int32_t y = 0; y < height; y++)
	{
		fprintf(stderr, "\rRendering (%d spp) %5.2f%%", samps * 4, 100. * y / (static_cast<int64_t>(height) - 1));

		// Loop cols
		for (uint16_t x = 0, Xi[3] = { 0, 0, static_cast<uint16_t>(y * y * y) }; x < width; x++)
		{
			for (int32_t sy = 0, i = (height - y - 1) * width + static_cast<int32_t>(x); sy < 2; sy++)
			{


				// 2x2 subpixel rows
				for (int32_t sx = 0; sx < 2; sx++, r = Vector3())
				{
					// 2x2 subpixel cols
					for (int32_t s = 0; s < samps; s++)
					{
						double r1 = 2 * erand48(Xi), dx = r1 < 1 ? sqrt(r1) - 1 : 1 - sqrt(2 - r1);
						double r2 = 2 * erand48(Xi), dy = r2 < 1 ? sqrt(r2) - 1 : 1 - sqrt(2 - r2);
						Vector3 d = xDirectionIncrement * (((sx + .5 + dx) / 2 + x) / width - .5) +
							yDirectionIncrement * (((sy + .5 + dy) / 2 + y) / height - .5) + cam.direction;

						//r = r + radiance(Ray(cam.origin + d * 140, d.normalise()), 0, Xi) * (1. / samps);

						r = r + trace(cam.origin + d * 140, d.normalise(), 50, 1);
					}

					// Camera rays are pushed ^^^^^ forward to start in interior
					image[i] = image[i] + Vector3(clamp(r.x), clamp(r.y), clamp(r.z)) * .25;
				}
			}
		}
	}

	FILE* f = fopen("image.ppm", "w"); // Write image to PPM file.
	fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);

	for (int32_t i = 0; i < width * height; i++)
	{
		fprintf(f, "%d %d %d ", toInt(image[i].x), toInt(image[i].y), toInt(image[i].z));
	}

	fclose(f);

	return 0;
}

