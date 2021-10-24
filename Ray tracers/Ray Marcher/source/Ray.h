#pragma once

#include "Vector3.h"

class Ray {
public:
	Vector3 origin;
	Vector3 direction;
	double distance;

	Ray() {}
	Ray(const Vector3& origin, const Vector3& direction)
		: origin(origin), direction(direction), distance(0)
	{}

	Ray(const Vector3& origin, const Vector3& direction, double distance)
		: origin(origin), direction(direction), distance(distance)
	{}

	Vector3 at(double t) const {
		return origin + direction * t;
	}

	Vector3 at() const {
		return at(distance);
	}
};


