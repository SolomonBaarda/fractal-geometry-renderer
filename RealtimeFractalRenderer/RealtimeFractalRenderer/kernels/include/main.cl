#ifndef MAIN_CL
/// @cond DOXYGEN_DO_NOT_DOCUMENT
#define MAIN_CL
/// @endcond

#ifndef MAXIMUM_MARCH_STEPS
/// <summary>Maximum number of iterations each ray can complete</summary>
/// <returns>int</returns>
#define MAXIMUM_MARCH_STEPS 100
#endif

#ifndef MAXIMUM_MARCH_DISTANCE
/// <summary>Maximum distance in world units that each ray can travel</summary>
/// <returns>float</returns>
#define MAXIMUM_MARCH_DISTANCE 1000.0f
#endif

#ifndef SURFACE_INTERSECTION_EPSILON
/// <summary>Epsilon value used to define the maximum distance which is considered a ray collision</summary>
/// <returns>float</returns>
#define SURFACE_INTERSECTION_EPSILON 0.00001f
#endif

#ifndef SURFACE_NORMAL_EPSILON
/// <summary>Epsilon value used to calculate surface the normal of geometry</summary>
/// <returns>float</returns>
#define SURFACE_NORMAL_EPSILON 0.001f
#endif

#ifndef CAMERA_VERTICAL_FOV_DEGREES
/// <summary>Camera vertical field of view in degrees</summary>
/// <returns>float</returns>
#define CAMERA_VERTICAL_FOV_DEGREES 40.0f
#endif 

#ifndef CAMERA_FOCUS_DISTANCE
/// <summary>Camera focus distance in world units</summary>
/// <returns>float</returns>
#define CAMERA_FOCUS_DISTANCE 0.1f
#endif

#ifndef CAMERA_UP_AXIS
/// <summary>Camera up axis. Must be a normalised float3.</summary>
/// <returns>float3</returns>
#define CAMERA_UP_AXIS (float3)(0, 1, 0)
#endif

#ifndef CAMERA_DO_LOOP
/// <summary>Whether the camera should return to the starting position once it reaches the end position. 
/// Only used if the camera is using a camera path</summary>
/// <returns>bool</returns>
#define CAMERA_DO_LOOP false
#endif

#ifndef BENCHMARK_START_STOP_TIME
/// <summary>The start and stop time that the benchmarker should use. If the values are negative, then 
/// the benchmarker will run for the entire durarion that the scene is active</summary>
/// <returns>float2</returns>
#define BENCHMARK_START_STOP_TIME (float2)(-1, -1)
#endif


// Throw compile time errors if these values have not been defined
// Define the macros so that Doxygen can see them, though these values will never be used

#ifndef CAMERA_POSITIONS_ARRAY
#error "CAMERA_POSITIONS_ARRAY must be defined"
/// <summary>
/// Array of camera world positions and times, used when calculating camera paths.
/// Each value in the array should be in the format (float4)(x, y, z, time). 
/// 
/// <ul>
/// <li>If it is defined as an empty array, then one default position will be used
/// <li>If it is defined as a single position, then the scene will use a controllable camera
/// <li>If it is defined as more than one position, then the scene will use a camera path
/// </ul>
/// 
/// CAMERA_POSITIONS_ARRAY and CAMERA_FACING_DIRECTIONS_ARRAY must both have only one element 
/// for the controllable camera to be selected.
/// </summary>
/// <returns>float4[]</returns>
#define CAMERA_POSITIONS_ARRAY { (float4)(0, 0, 0, 0) }
#endif

#ifndef CAMERA_FACING_DIRECTIONS_ARRAY
#error "CAMERA_FACING_DIRECTIONS_ARRAY must be defined"
/// <summary>
/// Array of camera normalised facing directions and times, used when calculating camera paths.
/// Each value in the array should be in the format (float4)(x, y, z, time). 
/// 
/// <ul>
/// <li>If it is defined as an empty array, then one default direction will be used
/// <li>If it is defined as a single direction, then the scene will use a controllable camera
/// <li>If it is defined as more than one direction, then the scene will use a camera path
/// </ul>
/// 
/// CAMERA_POSITIONS_ARRAY and CAMERA_FACING_DIRECTIONS_ARRAY must both have only one element 
/// for the controllable camera to be selected.
/// </summary>
/// <returns>float4[]</returns>
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(1, 1, 1)), 0.0f) }
#endif

#ifndef CAMERA_POSITIONS_LENGTH
#error "CAMERA_POSITIONS_LENGTH must be defined"
/// <summary>The length of the array specified by CAMERA_POSITIONS_ARRAY</summary>
/// <returns>int</returns>
#define CAMERA_POSITIONS_LENGTH 1
#endif

#ifndef CAMERA_FACING_DIRECTIONS_LENGTH
#error "CAMERA_FACING_DIRECTIONS_LENGTH must be defined"
/// <summary>The length of the array specified by CAMERA_FACING_DIRECTIONS_ARRAY</summary>
/// <returns>int</returns>
#define CAMERA_FACING_DIRECTIONS_LENGTH 1
#endif


/// <summary>
/// </summary>
/// <param name="camera_up_axis"></param>
/// <param name="array_capacity"></param>
/// <param name="number_camera_positions"></param>
/// <param name="camera_positions_at_time"></param>
/// <param name="number_camera_facing"></param>
/// <param name="camera_facing_at_time"></param>
/// <param name="do_camera_loop"></param>
/// <param name="benchmark_start_stop_time"></param>
/// <returns></returns>
__kernel void getSceneInformation(
	__global float3* camera_up_axis, __global float* camera_vertical_fov_degrees, 
	__global float* camera_focus_distance, const uint array_capacity,
	__global uint* number_camera_positions, __global float4* camera_positions_at_time,
	__global uint* number_camera_facing, __global float4* camera_facing_at_time,
	__global bool* do_camera_loop, __global float2* benchmark_start_stop_time)
{
	*camera_up_axis = CAMERA_UP_AXIS;
	*camera_vertical_fov_degrees = CAMERA_VERTICAL_FOV_DEGREES;
	*camera_focus_distance = CAMERA_FOCUS_DISTANCE;
	*number_camera_positions = CAMERA_POSITIONS_LENGTH;
	*number_camera_facing = CAMERA_FACING_DIRECTIONS_LENGTH;
	*benchmark_start_stop_time = BENCHMARK_START_STOP_TIME;
	*do_camera_loop = CAMERA_DO_LOOP;


	// Construct compile time arrays
	float4 positions[CAMERA_POSITIONS_LENGTH] = CAMERA_POSITIONS_ARRAY;
	float4 facing[CAMERA_FACING_DIRECTIONS_LENGTH] = CAMERA_FACING_DIRECTIONS_ARRAY;

	// Fill the arrays as much as we can
	for (int i = 0; i < CAMERA_POSITIONS_LENGTH && i < array_capacity; i++)
	{
		camera_positions_at_time[i] = positions[i];
	}
	for (int i = 0; i < CAMERA_FACING_DIRECTIONS_LENGTH && i < array_capacity; i++)
	{
		camera_facing_at_time[i] = facing[i];
	}
}


/// <summary>
/// A struct containing a position and normalised direction vector.
/// </summary>
typedef struct
{
	float3 position;
	float3 direction;
}
Ray;


/// <summary>
/// Calculates the surface normal for a any point in the scene.
/// </summary>
/// <param name="position">Position in world space</param>
/// <param name="time">Scene time in seconds</param>
/// <returns></returns>
float3 estimateSurfaceNormal(float3 position, float time)
{
	const float3 xOffset = (float3)(SURFACE_NORMAL_EPSILON, 0, 0);
	const float3 yOffset = (float3)(0, SURFACE_NORMAL_EPSILON, 0);
	const float3 zOffset = (float3)(0, 0, SURFACE_NORMAL_EPSILON);

	float x = signedDistanceEstimation(position + xOffset, time).w - signedDistanceEstimation(position - xOffset, time).w;
	float y = signedDistanceEstimation(position + yOffset, time).w - signedDistanceEstimation(position - yOffset, time).w;
	float z = signedDistanceEstimation(position + zOffset, time).w - signedDistanceEstimation(position - zOffset, time).w;

	return normalize((float3)(x, y, z));
}


/// <summary>
/// Traces the path of a ray.
/// </summary>
/// <param name="ray">A ray</param>
/// <param name="time">Scene time in seconds</param>
/// <returns>The colour that the pixel should be drawn as (range 0-1)</returns>
float3 trace(Ray ray, float time)
{
	float totalDistance;
	int steps;
	for (steps = 0, totalDistance = 0; steps < MAXIMUM_MARCH_STEPS && totalDistance < MAXIMUM_MARCH_DISTANCE; steps++)
	{
		float3 currentPosition = ray.position + (ray.direction * totalDistance);
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

/// <summary>
/// Creates a ray for the specified position on the camera screen.
/// </summary>
/// <param name="screenLowerLeftCorner"></param>
/// <param name="screenHorizontal"></param>
/// <param name="screenVertical"></param>
/// <param name="screen_coordinate">Position of this pixel on the screen relative to the whole screen, range 0-1 for x and y</param>
/// <param name="camera_position"></param>
/// <returns>A Ray</returns>
Ray getCameraRay(float3 screenLowerLeftCorner, float3 screenHorizontal, float3 screenVertical, float2 screen_coordinate, float3 camera_position)
{
	float3 screenPosition = screenLowerLeftCorner + screenHorizontal * screen_coordinate.x + screenVertical * screen_coordinate.y - camera_position;

	Ray r;
	r.position = camera_position + screenPosition;
	r.direction = normalise(screenPosition);
	return r;
}

/// <summary>
/// </summary>
/// <param name="colour"></param>
/// <returns></returns>
uchar3 convertColourTo8Bit(float3 colour)
{
	return (uchar3)((uchar)(clamp01(colour.x) * 255), (uchar)(clamp01(colour.y) * 255), (uchar)(clamp01(colour.z) * 255));
}







/// <summary>
/// </summary>
/// <param name="screen_coordinate"></param>
/// <param name="colours"></param>
/// <param name="total_number_of_pixels"></param>
/// <param name="time"></param>
/// <param name="camera_position"></param>
/// <param name="camera_facing"></param>
/// <param name="camera_aspect_ratio"></param>
/// <returns></returns>
__kernel void calculatePixelColour(
	__global const float2* screen_coordinate, __global uchar* colours, const uint total_number_of_pixels,
	const float time, const float3 camera_position, const float3 screenLowerLeftCorner, const float3 screenHorizontal, const float3 screenVertical)
{
	// Get gloabl thread ID
	int ID = get_global_id(0);
	int output_ID = ID * 4;

	// Make sure we are within the array size
	if (ID < total_number_of_pixels)
	{
		Ray ray = getCameraRay(screenLowerLeftCorner, screenHorizontal, screenVertical, screen_coordinate[ID], camera_position);

		float3 colour = trace(ray, time);
		uchar3 colour_8_bit = convertColourTo8Bit(colour);

		colours[output_ID] = colour_8_bit.r;
		colours[output_ID + 1] = colour_8_bit.g;
		colours[output_ID + 2] = colour_8_bit.b;
		colours[output_ID + 3] = 255;
	}
}

#endif