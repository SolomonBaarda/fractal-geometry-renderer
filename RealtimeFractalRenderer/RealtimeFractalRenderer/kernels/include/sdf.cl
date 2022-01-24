#ifndef SDF_CL
#define SDF_CL

#include "utils.cl"

/// <summary>
/// </summary>
/// <param name=""></param>
float sphereSDF(float3 position, float3 centre, float radius)
{
	return magnitude(centre - position) - radius;
}

/// <summary>
/// </summary>
/// <param name=""></param>
float boxSDF(float3 position, float3 centre, float3 dimensions)
{
	float3 q = absolute(centre - position) - dimensions;
	float length = magnitude((float3)(max(q.x, 0.0f), max(q.y, 0.0f), max(q.z, 0.0f)));
	return length + min(max(q.x, max(q.y, q.z)), 0.0f);
}

#endif