// https://www.kevinbeason.com/smallpt/

#include <math.h>   // smallpt, a Path Tracer by Kevin Beason, 2008
#include <stdlib.h> // Make : g++ -O3 -fopenmp smallpt.cpp -o smallpt
#include <stdio.h>  //        Remove "-fopenmp" for g++ version < 4.2
#include <random>
#include <iostream>

#include "structs.h"

#include "render.h"




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
	const int32_t width = 1920, height = static_cast<int>(width / aspect_ratio);
	Vector3* image = new Vector3[static_cast<int64_t>(width) * static_cast<int64_t>(height)];

	// Camera
	const float viewport_height = 2.0f;
	const float viewport_width = aspect_ratio * viewport_height;
	const float focal_length = 1.0f;

	const Vector3 origin(0, 0, 2);
	const Vector3 horizontal(viewport_width, 0, 0);
	const Vector3 vertical(0, viewport_height, 0);
	const Vector3 lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vector3(0, 0, focal_length);


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

			Vector3 screenPosition = lower_left_corner + horizontal * u + vertical * v - origin;

			// Ray march from the screen position in the ray direction
			image[y * width + x] = render(origin + screenPosition, screenPosition.normalised());
		}
	}

	saveImageToFile(image, width, height, "image.ppm");

	return 0;
}

