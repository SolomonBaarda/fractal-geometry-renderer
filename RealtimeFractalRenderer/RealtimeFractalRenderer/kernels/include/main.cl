#ifndef MAIN_CL
#define MAIN_CL

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

float3 estimateSurfaceNormal(float3 surface, float time)
{
	float3 xOffset = (float3)(SURFACE_NORMAL_EPSILON, 0, 0);
	float3 yOffset = (float3)(0, SURFACE_NORMAL_EPSILON, 0);
	float3 zOffset = (float3)(0, 0, SURFACE_NORMAL_EPSILON);

	float x = signedDistanceEstimation(surface + xOffset, time).w - signedDistanceEstimation(surface - xOffset, time).w;
	float y = signedDistanceEstimation(surface + yOffset, time).w - signedDistanceEstimation(surface - yOffset, time).w;
	float z = signedDistanceEstimation(surface + zOffset, time).w - signedDistanceEstimation(surface - zOffset, time).w;

	return normalize((float3)(x, y, z));
}

float3 trace(float3 pos, float3 dir, float time)
{
	float totalDistance;
	int steps;
	for (steps = 0, totalDistance = 0; steps < MAXIMUM_MARCH_STEPS && totalDistance < MAXIMUM_MARCH_DISTANCE; steps++)
	{
		float3 currentPosition = pos + (float3)(dir.x * totalDistance, dir.y * totalDistance, dir.z * totalDistance);
		float4 colourAndDistance = signedDistanceEstimation(currentPosition, time);
		totalDistance += colourAndDistance.w;

		// Hit the surface of an object
		if (colourAndDistance.w <= SURFACE_INTERSECTION_EPSILON)
		{
			float3 new_colour = colourAndDistance.xyz;

			float3 normal = estimateSurfaceNormal(currentPosition, time);
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

typedef struct
{
	float3 position;
	float3 direction;
}
Ray;

Ray getCameraRay(float2 screen_coordinate, float3 camera_position, float3 camera_facing, float3 camera_up,
	float vertical_fov_degrees, float aspect_ratio, float focus_distance)
{
	float theta = degreesToRadians(vertical_fov_degrees);
	float h = tan(theta / 2);
	float viewport_height = 2.0 * h;
	float viewport_width = aspect_ratio * viewport_height;

	float3 w = normalise(camera_facing);
	float3 u = normalise(crossProduct(camera_up, w));
	float3 v = crossProduct(w, u);

	float3 horizontal = u * focus_distance * viewport_width;
	float3 vertical = v * focus_distance * viewport_height;
	float3 screenLowerLeftCorner = camera_position - horizontal / 2 - vertical / 2 - w * focus_distance;

	float3 screenPosition = screenLowerLeftCorner + horizontal * screen_coordinate.x + vertical * screen_coordinate.y - camera_position;

	Ray r;
	r.position = camera_position + screenPosition;
	r.direction = normalise(screenPosition);
	return r;
}

/// <summary>
/// convertColourTo8Bit a
/// </summary>
/// <param name="colour"></param>
uchar3 convertColourTo8Bit(float3 colour)
{
	return (uchar3)((uchar)(clamp01(colour.x) * 255), (uchar)(clamp01(colour.y) * 255), (uchar)(clamp01(colour.z) * 255));
}

/// <summary>
/// calculatePixelColour a
/// </summary>
/// <param name="screen_coordinate"></param>
/// <param name="colours"></param>
/// <param name="total_number_of_pixels"></param>
/// <param name="time"></param>
/// <param name="camera_position"></param>
/// <param name="camera_facing"></param>
/// <param name="camera_vertical_fov_degrees"></param>
/// <param name="camera_aspect_ratio"></param>
/// <param name="camera_focus_distance"></param>
__kernel void calculatePixelColour(
	__global const float2* screen_coordinate, __global uchar* colours, const uint total_number_of_pixels,
	const float time, const float3 camera_position, const float3 camera_facing, const float camera_vertical_fov_degrees,
	const float camera_aspect_ratio, const float camera_focus_distance)
{
	// Get gloabl thread ID
	int ID = get_global_id(0);
	int output_ID = ID * 4;

	const float3 camera_up = (float3)(0, 1, 0);

	// Make sure we are within the array size
	if (ID < total_number_of_pixels)
	{
		Ray r = getCameraRay(screen_coordinate[ID], camera_position, camera_facing, camera_up,
			camera_vertical_fov_degrees, camera_aspect_ratio, camera_focus_distance);

		float3 colour = trace(r.position, r.direction, time);
		uchar3 colour_8_bit = convertColourTo8Bit(colour);

		colours[output_ID] = colour_8_bit.x;
		colours[output_ID + 1] = colour_8_bit.y;
		colours[output_ID + 2] = colour_8_bit.z;
		colours[output_ID + 3] = 255;
	}
}

#endif