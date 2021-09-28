#include <math.h>   // smallpt, a Path Tracer by Kevin Beason, 2008
#include <stdlib.h> // Make : g++ -O3 -fopenmp smallpt.cpp -o smallpt
#include <stdio.h>  //        Remove "-fopenmp" for g++ version < 4.2
#include <random>
#include <iostream>

#include "structs.h"

#ifndef M_PI
#define M_PI 3.14159265359
#endif

#ifndef M_1_PI
#define M_1_PI (1.0 / M_PI)
#endif

const Sphere spheres[] =
{
	//Scene: radius, position, emission, color, material
	Sphere(1e5, Vector3(1e5 + 1, 40.8, 81.6), Vector3(), Vector3(.75, .25, .25), ReflectionType::DIFFUSE),   //Left
	Sphere(1e5, Vector3(-1e5 + 99, 40.8, 81.6), Vector3(), Vector3(.25, .25, .75), ReflectionType::DIFFUSE), //Rght
	Sphere(1e5, Vector3(50, 40.8, 1e5), Vector3(), Vector3(.75, .75, .75), ReflectionType::DIFFUSE),         //Back
	Sphere(1e5, Vector3(50, 40.8, -1e5 + 170), Vector3(), Vector3(), ReflectionType::DIFFUSE),               //Frnt
	Sphere(1e5, Vector3(50, 1e5, 81.6), Vector3(), Vector3(.75, .75, .75), ReflectionType::DIFFUSE),         //Botm
	Sphere(1e5, Vector3(50, -1e5 + 81.6, 81.6), Vector3(), Vector3(.75, .75, .75), ReflectionType::DIFFUSE), //Top
	Sphere(16.5, Vector3(27, 16.5, 47), Vector3(), Vector3(1, 1, 1) * .999, ReflectionType::SPECULAR),        //Mirr
	Sphere(16.5, Vector3(73, 16.5, 78), Vector3(), Vector3(1, 1, 1) * .999, ReflectionType::REFRACTIVE),        //Glas
	Sphere(600, Vector3(50, 681.6 - .27, 81.6), Vector3(12, 12, 12), Vector3(), ReflectionType::DIFFUSE)     //Lite
};

const std::uniform_real_distribution<double> distr(0.0, 1.0);

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

inline bool getClosestIntersectionWithSpheres(const Ray& r, double& t, int& id)
{
	double n = sizeof(spheres) / sizeof(Sphere), d, inf = t = 1e20;
	for (int32_t i = int(n); i--;)
	{
		if ((d = spheres[i].intersect(r)) && d < t)
		{
			t = d;
			id = i;
		}
	}
	return t < inf;
}

Vector3 radiance(const Ray& r, int32_t depth, uint16_t* Xi)
{
	double t;   // distance to intersection
	int32_t id = 0; // id of intersected object

	if (!getClosestIntersectionWithSpheres(r, t, id))
		return Vector3();                  // if miss, return black

	const Sphere& obj = spheres[id]; // the hit object
	Vector3 x = r.origin + r.direction * t, n = (x - obj.centre).normalise(), nl = n.dotProduct(r.direction) < 0 ? n : n * -1;

	Vector3 f = obj.colour;

	double maxRefl = std::max(obj.colour.x, std::max(obj.colour.y, obj.colour.z));
	//double p = obj.colour.x > obj.colour.y && obj.colour.x > obj.colour.z ? obj.colour.x : obj.colour.y > obj.colour.z ? obj.colour.y : obj.colour.z; // max refl

	if (++depth > 5)
	{
		return obj.emission; //R.R.

		if (erand48(Xi) < maxRefl)
		{
			f = f * (1 / maxRefl);
		}
		else
		{
			return obj.emission; //R.R.
		}
	}

	if (obj.reflection == ReflectionType::DIFFUSE)
	{
		// Ideal Refl_t::DIFFUSE reflection
		double r1 = 2 * M_PI * erand48(Xi), r2 = erand48(Xi), r2s = sqrt(r2);
		Vector3 w = nl, u = ((fabs(w.x) > .1 ? Vector3(0, 1) : Vector3(1)) % w).normalise(), v = w % u;
		Vector3 d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).normalise();

		return obj.emission + f.multiply(radiance(Ray(x, d), depth, Xi));
	}
	else if (obj.reflection == ReflectionType::SPECULAR)
	{
		// Ideal Refl_t::SPECULAR reflection
		return obj.emission + f.multiply(radiance(Ray(x, r.direction - n * 2 * n.dotProduct(r.direction)), depth, Xi));
	}

	Ray reflRay(x, r.direction - n * 2 * n.dotProduct(r.direction)); // Ideal dielectric Refl_t::REFRACTION
	bool into = n.dotProduct(nl) > 0;                // Ray from outside going in?
	double nc = 1, nt = 1.5, nnt = into ? nc / nt : nt / nc, ddn = r.direction.dotProduct(nl), cos2t;

	if ((cos2t = 1 - nnt * nnt * (1 - ddn * ddn)) < 0)
	{
		// Total internal reflection
		return obj.emission + f.multiply(radiance(reflRay, depth, Xi));
	}

	Vector3 tdir = (r.direction * nnt - n * ((into ? 1 : -1) * (ddn * nnt + sqrt(cos2t)))).normalise();
	double a = nt - nc, b = nt + nc, R0 = a * a / (b * b), c = 1 - (into ? -ddn : tdir.dotProduct(n));
	double Re = R0 + (1 - R0) * c * c * c * c * c, Tr = 1 - Re, P = .25 + .5 * Re, RP = Re / P, TP = Tr / (1 - P);

	return obj.emission + f.multiply(depth > 2 ? (erand48(Xi) < P ? // Russian roulette
		radiance(reflRay, depth, Xi) * RP
		: radiance(Ray(x, tdir), depth, Xi) * TP)
		: radiance(reflRay, depth, Xi) * Re + radiance(Ray(x, tdir), depth, Xi) * Tr);
}

int main(int argc, char* argv[])
{
	int32_t width = 1024, height = 768;
	//int32_t samps = argc == 2 ? atoi(argv[1]) / 4 : 1; // # samples
	int32_t samps = 100;

	Ray cam(Vector3(50, 52, 295.6), Vector3(0, -0.042612, -1).normalise());        // cam pos, dir

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
						r = r + radiance(Ray(cam.origin + d * 140, d.normalise()), 0, Xi) * (1. / samps);
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

