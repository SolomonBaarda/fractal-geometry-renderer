#ifndef SDF_CL
/// @cond DOXYGEN_DO_NOT_DOCUMENT
#define SDF_CL

#include "utils.cl"

float sphereSDF(const float3 position, const float3 centre, const float radius)
{
	return magnitude(centre - position) - radius;
}

float boxSDF(const float3 position, const float3 centre, const float3 dimensions)
{
	float3 q = absolute(centre - position) - dimensions;
	float length = magnitude((float3)(max(q.x, 0.0f), max(q.y, 0.0f), max(q.z, 0.0f)));
	return length + min(max(q.x, max(q.y, q.z)), 0.0f);
}

float opUnion(const float d1, const float d2)
{
	return min(d1, d2);
}

float opSubtraction(const float d1, const float d2)
{
	return max(-d1, d2);
}

float opIntersection(const float d1, const float d2)
{
	return max(d1, d2);
}

float opSmoothUnion(const float d1, const float d2, const float k)
{
	float h = max(k - f_abs(d1 - d2), 0.0f);
	return min(d1, d2) - h * h * 0.25f / k;
}

float opPolynomialSmoothUnion(const float a, const float b, const float k)
{
	float h = max(k - f_abs(a - b), 0.0f) / k;
	return min(a, b) - h * h * k * (1.0f / 4.0f);
}

float opSmoothSubtraction(const float d1, const float d2, const float k)
{
	float h = max(k - f_abs(-d1 - d2), 0.0f);
	return max(-d1, d2) + h * h * 0.25f / k;
}

float opSmoothIntersection(const float d1, const float d2, const float k)
{
	float h = max(k - f_abs(d1 - d2), 0.0f);
	return max(d1, d2) + h * h * 0.25 / k;
}


bool isWithinBoundingSphere(const float3 position, const float3 sphereCentre, const float sphereRadius)
{
	return magnitude(sphereCentre - position) - sphereRadius <= 0;
}


#endif