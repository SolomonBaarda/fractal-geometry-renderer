#include "camera.h"
#include "color.h"
#include "vec3.h"
#include "ray.h"

#include <iostream>
#include <fstream>

float sphereSDF(vec3 point, vec3 sphereCentre, float sphereRadius)
{
	return (sphereCentre - point).length() - sphereRadius;
}

float signedDistanceEstimation(vec3 point)
{
	return sphereSDF(point, vec3(0,0,0), 0.25f);

}



color trace(ray r, const uint32_t maximumRaySteps, const uint32_t minimumDistancePerIteration)
{
	// http://blog.hvidtfeldts.net/index.php/2011/06/distance-estimated-3d-fractals-part-i/

	float totalDistance = 0.0;
	int steps;

	for (steps = 0; steps < maximumRaySteps; steps++)
	{
		vec3 position = r.orig + (r.dir * totalDistance);
		float distanceEstimation = signedDistanceEstimation(position);
		totalDistance += distanceEstimation;

		if (distanceEstimation < minimumDistancePerIteration)
		{
			return color(1, 1, 1);
			break;
		}
	}

	float c = 1.0 - float(steps) / float(maximumRaySteps);

	return color(0,0,0);
}


int main() {

	std::ofstream file ("image.ppm", std::ios::trunc);

	// Image

	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 1200; // was 1200
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 10;
	const int max_depth = 50;

	// World



	// Camera

	point3 lookfrom(10, 0, 0);
	point3 lookat(0, 0, 0);
	vec3 vup(0, 1, 0);
	auto dist_to_focus = 10.0;
	auto aperture = 0.1;

	camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

	// Render

	file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

//#pragma omp parallel for schedule(dynamic, 1) 

	for (int y = image_height - 1; y >= 0; --y)
	{
		std::cerr << "\rScanlines remaining: " << y << ' ' << std::flush;
		for (int x = 0; x < image_width; ++x)
		{
			color pixel_color(0, 0, 0);
			//for (int s = 0; s < samples_per_pixel; ++s)
			{
				//auto u = (i + random_double()) / (image_width - 1);
				//auto v = (j + random_double()) / (image_height - 1);
				ray r = cam.get_ray(x, y);


				pixel_color += trace(r, max_depth, 1);
			}

			// normal
			//vec3 n = normalize(vec3(DE(pos + xDir) - DE(pos - xDir), DE(pos + yDir) - DE(pos - yDir), DE(pos + zDir) - DE(pos - zDir)));

			write_color(file, pixel_color, samples_per_pixel);
		}
	}

	file.close();

	std::cerr << "\nDone.\n";
}
