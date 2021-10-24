#pragma once

#include <cmath>
#include "structs.h"

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline float degrees_to_radians(float degrees) {
	return degrees * pi / 180.0;
}

class Camera {
public:
	Camera() : Camera(Vector3(0, 0, -1), Vector3(0, 0, 0), Vector3(0, 1, 0), 40, 1, 0, 10) {}

	Camera(
		Vector3 lookfrom,
		Vector3 lookat,
		Vector3   vup,
		float vfov, // vertical field-of-view in degrees
		float aspect_ratio,
		float aperture,
		float focus_dist
	) {
		auto theta = degrees_to_radians(vfov);
		auto h = tan(theta / 2);
		auto viewport_height = 2.0 * h;
		auto viewport_width = aspect_ratio * viewport_height;

		w = (lookfrom - lookat).normalise();
		u = Vector3::crossProduct(vup, w).normalise();
		v = Vector3::crossProduct(w, u);

		origin = lookfrom;
		horizontal = u * focus_dist * viewport_width;
		vertical = v * focus_dist * viewport_height;
		lower_left_corner = origin - horizontal / 2 - vertical / 2 - w * focus_dist;

		lens_radius = aperture / 2;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="x">0 to 1 value for screen space</param>
	/// <param name="y">0 to 1 value for screen space</param>
	/// <returns></returns>
	Ray getRay(float x, float y) const
	{
		Vector3 screenPosition = lower_left_corner + horizontal * x + vertical * y - origin;
		return Ray(origin + screenPosition, screenPosition.normalised());
	}

private:
	Vector3 origin;
	Vector3 lower_left_corner;
	Vector3 horizontal;
	Vector3 vertical;
	Vector3 u, v, w;
	float lens_radius;
};

