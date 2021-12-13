#ifndef MAXIMUM_MARCH_STEPS
#define MAXIMUM_MARCH_STEPS 100
#endif

#ifndef MAXIMUM_MARCH_DISTANCE
#define MAXIMUM_MARCH_DISTANCE 1000.0f
#endif

#ifndef SURFACE_INTERSECTION_EPSILON
#define SURFACE_INTERSECTION_EPSILON 0.00001f
#endif

#ifndef SURFACE_NORMAL_EPSILON
#define SURFACE_NORMAL_EPSILON 0.001f
#endif


float magnitude(float3 vec)
{
	return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

float3 absolute(float3 value)
{
	return (float3)(
		value.x < 0 ? -1 * value.x : value.x,
		value.y < 0 ? -1 * value.y : value.y,
		value.z < 0 ? -1 * value.z : value.z
		);
}

float sphereSDF(float3 position, float3 centre, float radius)
{
	return magnitude(centre - position) - radius;
}

float boxSDF(float3 position, float3 centre, float3 dimensions)
{
	float3 q = absolute(centre - position) - dimensions;
	float length = magnitude((float3)(max(q.x, 0.0f), max(q.y, 0.0f), max(q.z, 0.0f)));
	return length + min(max(q.x, max(q.y, q.z)), 0.0f);
}

float4 signedDistanceEstimation(float3 position)
{
	float distance = min(
		sphereSDF(position, (float3)(0, 0, 0), 3.5f),
		boxSDF(position, (float3)(0, 0, 0), (float3)(4, 0.5f, 4))
	);

	return (float4)((float3)(0.5f, 0.5f, 0.5f), distance);
}

float3 estimateSurfaceNormal(float3 surface)
{
	float3 xOffset = (float3)(SURFACE_NORMAL_EPSILON, 0, 0);
	float3 yOffset = (float3)(0, SURFACE_NORMAL_EPSILON, 0);
	float3 zOffset = (float3)(0, 0, SURFACE_NORMAL_EPSILON);

	float x = signedDistanceEstimation(surface + xOffset).w - signedDistanceEstimation(surface - xOffset).w;
	float y = signedDistanceEstimation(surface + yOffset).w - signedDistanceEstimation(surface - yOffset).w;
	float z = signedDistanceEstimation(surface + zOffset).w - signedDistanceEstimation(surface - zOffset).w;

	return normalize((float3)(x, y, z));
}


__kernel void calculatePixelColour(__global const float3* position, __global const float3* direction,
	__global float3* colour, const unsigned int max_length)
{
	// Get gloabl thread ID
	int ID = get_global_id(0);

	// Make sure we are within the array size
	if (ID < max_length)
	{
		float3 dir = direction[ID];
		float3 pos = position[ID];

		float totalDistance;
		int steps;
		for (steps = 0, totalDistance = 0; steps < MAXIMUM_MARCH_STEPS && totalDistance < MAXIMUM_MARCH_DISTANCE; steps++)
		{
			float3 currentPosition = pos + (float3)(dir.x * totalDistance, dir.y * totalDistance, dir.z * totalDistance);
			float4 colourAndDistance = signedDistanceEstimation(currentPosition);
			totalDistance += colourAndDistance.w;

			// Hit the surface of an object
			if (colourAndDistance.w <= SURFACE_INTERSECTION_EPSILON)
			{
				float3 new_colour = colourAndDistance.xyz;

				float3 normal = estimateSurfaceNormal(currentPosition);
				float percent = (float)steps / (float)MAXIMUM_MARCH_STEPS;

				// Render normals
				new_colour = (normal + (float3)(1)) * 0.5f;

				// Non-shaded
				new_colour = new_colour * (1 - percent);

				// Phong
				//colour = Vector3::multiplyComponents(colour , phong(normal, direction));

				colour[ID] = new_colour;
				return;
			}
		}

		colour[ID] = (float3)(0);
	}
}
