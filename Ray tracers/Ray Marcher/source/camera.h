#pragma once

#include <cmath>
#include "structs.h"

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline float degrees_to_radians(float degrees) {
	return degrees * pi / 180.0;
}

class Camera
{
public:
	Camera() : Camera(Vector3(0, 0, -1), Vector3(0, 0, 0), Vector3(0, 1, 0), 90.0f, 16.0f / 9.0f, 1.0f) {}

	Vector3 position;
	Vector3 lookat;
	Vector3 verticalUp;
	float verticalFOVDegrees;
	float aspectRatio;
	float focusDistance;

	Camera(Vector3 position, Vector3 lookat, Vector3 verticalUp, float verticalFOVDegrees,
		float aspectRatio, float focusDistance
	) : position(position), lookat(lookat), verticalUp(verticalUp), verticalFOVDegrees(verticalFOVDegrees),
		aspectRatio(aspectRatio), focusDistance(focusDistance)
	{
		recalculateValues();
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="x">0 to 1 value for screen space coordinate</param>
	/// <param name="y">0 to 1 value for screen space coordinate</param>
	/// <returns></returns>
	Ray getCameraRay(float x, float y) const
	{
		Vector3 screenPosition = screenLowerLeftCorner + horizontal * x + vertical * y - position;
		return Ray(position + screenPosition, screenPosition.normalised());
	}

private:
	Vector3 screenLowerLeftCorner;
	Vector3 horizontal;
	Vector3 vertical;
	Vector3 u, v, w;

	void recalculateValues()
	{
		float theta = degrees_to_radians(verticalFOVDegrees);
		float h = tan(theta / 2);
		float viewport_height = 2.0 * h;
		float viewport_width = aspectRatio * viewport_height;

		w = (position - lookat).normalise();
		u = Vector3::crossProduct(verticalUp, w).normalise();
		v = Vector3::crossProduct(w, u);

		horizontal = u * focusDistance * viewport_width;
		vertical = v * focusDistance * viewport_height;
		screenLowerLeftCorner = position - horizontal / 2 - vertical / 2 - w * focusDistance;
	}
};

