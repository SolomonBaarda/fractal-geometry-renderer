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

float3 trace(float3 pos, float3 dir)
{
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

			return new_colour;
		}
	}

	return (float3)(0);
}

float clamp01(float a)
{
	return a < 0 ? 0 : a > 1 ? 1 : a;
}

uchar3 convertColourTo8Bit(float3 colour)
{
	return (uchar3)(clamp01(colour.x) * 255, clamp01(colour.y) * 255, clamp01(colour.z) * 255);
}

__kernel void calculatePixelColour(__global const float3* positions, __global const float3* directions,
	__global uchar* colours, const uint max_length)
{
	// Get gloabl thread ID
	int ID = get_global_id(0);
	int output_ID = ID * 4;

	// Make sure we are within the array size
	if (ID < max_length)
	{
		float3 colour = trace(positions[ID], directions[ID]);
		uchar3 colour_8_bit = convertColourTo8Bit(colour);

		colours[output_ID] = colour_8_bit.x;
		colours[output_ID + 1] = colour_8_bit.y;
		colours[output_ID + 2] = colour_8_bit.z;
		colours[output_ID + 3] = 255;
	}
}
