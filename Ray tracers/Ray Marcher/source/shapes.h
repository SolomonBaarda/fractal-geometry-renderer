#include "structs.h"
#include "signedDistanceFunctions.h"

#ifndef SHAPES
#define SHAPES

class iShape
{
public:
	virtual float signedDistanceEstimation(const Vector3& position) = 0;
};


class Sphere : public iShape
{
public:
	Vector3 centre;
	float radius;

	Sphere(Vector3 centre, float radius) : centre(centre), radius(radius)
	{}

	virtual float signedDistanceEstimation(const Vector3& position) override
	{
		return sphereSDF(position, centre, radius);
	}
};

#endif
