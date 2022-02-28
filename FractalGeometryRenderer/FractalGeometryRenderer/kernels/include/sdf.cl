#ifndef SDF_CL
#define SDF_CL

#include "utils.cl"

/// <summary>
/// </summary>
/// <param name=""></param>
float sphereSDF(const float3 position, const float3 centre, const float radius)
{
	return magnitude(centre - position) - radius;
}

/// <summary>
/// </summary>
/// <param name=""></param>
float boxSDF(const float3 position, const float3 centre, const float3 dimensions)
{
	float3 q = absolute(centre - position) - dimensions;
	float length = magnitude((float3)(max(q.x, 0.0f), max(q.y, 0.0f), max(q.z, 0.0f)));
	return length + min(max(q.x, max(q.y, q.z)), 0.0f);
}

//float cylinderSDF(const float3 position, const float3 centre)
//{
//	return magnitude((float2)(position.x, position.z) - (float2)(centre.x, centre.y) - centre.z);
//}

//float sdOctahedron(const float3 position, const float s)
//{
//	const float3 p = abs(position);
//	float m = p.x + p.y + p.z - s;
//
//	vec3 q;
//	if (3.0 * p.x < m) q = p.xyz;
//	else if (3.0 * p.y < m) q = p.yzx;
//	else if (3.0 * p.z < m) q = p.zxy;
//	else return m * 0.57735027;
//
//	float k = clamp(0.5 * (q.z - q.y + s), 0.0, s);
//	return length(vec3(q.x, q.y - s + k, q.z - k));
//}




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