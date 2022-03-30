#define OVERRIDE true

#ifndef ITERATIONS
#define ITERATIONS 10
#endif

#define USE_BOUNDING_VOLUME false

#include "utils.cl"
#include "types.cl"
#include "sdf.cl"

Material mandelbulbSDF(const float3 position, const float time, float* distance)
{
	// Material
	Material material;

	//if (position.x > 0)
	//{
	//	*distance = position.x;
	//	return material;
	//}

	const float power = 7.75f + time * 0.01f;

	float3 w = position;
	float m = dot(w, w);
	float4 colorParams = (float4) (absolute(w), m);
	float dz = 1.0f;

	for (int i = 0; i < ITERATIONS; i++)
	{
		dz = 8.0f * pow(sqrt(m), 7.0f) * dz + 1.0f;

		// Calculate power
		float r = length(w);
		float b = power * acos(w.y / r);
		float a = power * atan2(w.x, w.z);
		w = pow(r, power) * (float3) (sin(b) * sin(a), cos(b), sin(b) * cos(a)) + position;

		colorParams = min(colorParams, (float4) (absolute(w), m));
		m = dot(w, w);

		if (m > 256.0f) break;
	}

	material.ambient = (float3)(colorParams.x, colorParams.y, colorParams.z);
	material.diffuse = material.ambient;
	material.specular = (float3)(0.5f, 0.5f, 0.5f);
	material.shininess = 50.0f;

	// Distance estimation
	*distance = 0.25f * log(m) * sqrt(m) / dz;

	return material;
}

#include "mandelbulb.cl"

