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
#define SURFACE_INTERSECTION_EPSILON 0.001f
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

/// <summary>Background colour for the scene</summary>
/// <returns>float3</returns>
#ifndef SCENE_BACKGROUND_COLOUR
#define SCENE_BACKGROUND_COLOUR (float3)(0)
#endif




#ifndef SCENE_LIGHT_COLOUR
#define SCENE_LIGHT_COLOUR (float3)(1)
#endif

/// <summary>Position of the main light in the scene</summary>
/// <returns>float3</returns>
#ifndef SCENE_LIGHT_POSITION
#define SCENE_LIGHT_POSITION (float3)(0)
#endif

/// <summary>Epsilon value used when calculating shadows on geometry. Represents the distance at which 
/// shadow checks start being made. Must be greater than SURFACE_INTERSECTION_EPSILON to prevent artifacting</summary>
/// <returns>float</returns>
#ifndef SURFACE_SHADOW_EPSILON
#define SURFACE_SHADOW_EPSILON 0.1f
#endif

/// <summary>Falloff value used when calculating soft shadows</summary>
/// <returns>float</returns>
#ifndef SURFACE_SHADOW_FALLOFF
#define SURFACE_SHADOW_FALLOFF 1.0f
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
/// Kernel function used to pass scene information to the C++ interface.
/// </summary>
/// <param name="camera_up_axis"></param>
/// <param name="array_capacity"></param>
/// <param name="number_camera_positions"></param>
/// <param name="camera_positions_at_time"></param>
/// <param name="number_camera_facing"></param>
/// <param name="camera_facing_at_time"></param>
/// <param name="do_camera_loop"></param>
/// <param name="benchmark_start_stop_time"></param>
__kernel void getSceneInformation(
	__global float3* camera_up_axis, const uint array_capacity,
	__global uint* number_camera_positions, __global float4* camera_positions_at_time,
	__global uint* number_camera_facing, __global float4* camera_facing_at_time,
	__global bool* do_camera_loop, __global float2* benchmark_start_stop_time)
{
	*camera_up_axis = CAMERA_UP_AXIS;
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
/// <returns>Surface normal vector for the position on the geometry</returns>
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
/// Calculates soft shadows for a point on geometry in the scene.
/// </summary>
/// <param name="pointOnGeometry">Position in world space</param>
/// <param name="time">Scene time in seconds</param>
/// <returns>Shadow value</returns>
float calculateShadow(float3 pointOnGeometry, float time)
{
	// https://www.iquilezles.org/www/articles/rmshadows/rmshadows.htm

	float shadow = 1.0f;
	float ph = 1e20;

	float3 distanceVector = SCENE_LIGHT_POSITION - pointOnGeometry;
	float3 directionFromGeometryToLight = normalise(distanceVector);
	float distanceFromGeometryToLight = magnitude(distanceVector);

	for (float totalDistance = SURFACE_SHADOW_EPSILON; totalDistance < distanceFromGeometryToLight; )
	{
		float3 currentPosition = pointOnGeometry + directionFromGeometryToLight * totalDistance;
		float4 colourAndDistance = signedDistanceEstimation(currentPosition, time);

		// Hit the surface of an object
		// Therefore the original point must be in shadow
		if (colourAndDistance.w <= SURFACE_INTERSECTION_EPSILON)
		{
			return 0.0f;
		}

		// Calculate soft shadow values
		float y = colourAndDistance.w * colourAndDistance.w / (2.0f * ph);
		float d = sqrt(colourAndDistance.w * colourAndDistance.w - y * y);
		shadow = min(shadow, SURFACE_SHADOW_FALLOFF * d / max(0.0f, totalDistance - y));
		ph = colourAndDistance.w;

		totalDistance += colourAndDistance.w;
	}

	return shadow;
}

float lambertianReflectance(float3 normal, float3 lightDirection)
{
	return max(dotProduct(normal, lightDirection), 0.0f);
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
			float3 colour = colourAndDistance.xyz;
			float3 normal = estimateSurfaceNormal(currentPosition, time);

			// Set colour to be surface normal
#ifdef RENDER_NORMALS

			colour = (normal + (float3)(1)) * 0.5f;
#endif

			// Apply lighting, shadows etc to surface colour
#ifdef DO_LIGHTING
			float shadow = calculateShadow(currentPosition, time);
			float lambert = lambertianReflectance(normal, normalise(SCENE_LIGHT_POSITION - currentPosition));

			colour = shadow * lambert * SCENE_LIGHT_COLOUR * colour;
#endif

			// Otherwise apply basic shading at edge of object 
#if !defined RENDER_NORMALS && !defined DO_LIGHTING
			float percent = (float)steps / (float)MAXIMUM_MARCH_STEPS;
			colour = colour * (1 - percent);
#endif

			return colour;
		}
	}

	return SCENE_BACKGROUND_COLOUR;
}

/// <summary>
/// Creates a ray for the specified position on the camera screen.
/// </summary>
/// <param name="screen_coordinate">Position on the screen, range 0-1 for x and y</param>
/// <param name="camera_position">Camera position in world units</param>
/// <param name="camera_facing">Camera normalised facing direction</param>
/// <param name="aspect_ratio"></param>
/// <returns>A Ray</returns>
Ray getCameraRay(float2 screen_coordinate, float3 camera_position, float3 camera_facing, float aspect_ratio)
{
	const float theta = CAMERA_VERTICAL_FOV_DEGREES * PI / 180.0f;
	const float h = tan(theta / 2);
	const float viewport_height = 2.0f * h;
	const float viewport_width = aspect_ratio * viewport_height;

	float3 w = normalise(camera_facing);
	float3 u = normalise(crossProduct(CAMERA_UP_AXIS, w));
	float3 v = crossProduct(w, u);

	float3 horizontal = u * CAMERA_FOCUS_DISTANCE * viewport_width;
	float3 vertical = v * CAMERA_FOCUS_DISTANCE * viewport_height;
	float3 screenLowerLeftCorner = camera_position - horizontal / 2 - vertical / 2 - w * CAMERA_FOCUS_DISTANCE;

	float3 screenPosition = screenLowerLeftCorner + horizontal * screen_coordinate.x + vertical * screen_coordinate.y - camera_position;

	Ray r;
	r.position = camera_position + screenPosition;
	r.direction = normalise(screenPosition);
	return r;
}

/// <summary>
/// Converts a colour range 0-1 to an integer colour with range 0-255.
/// </summary>
/// <param name="colour"></param>
/// <returns>An 8-bit colour value, range 0-255</returns>
uchar3 convertColourTo8Bit(float3 colour)
{
	return (uchar3)((uchar)(clamp01(colour.x) * 255), (uchar)(clamp01(colour.y) * 255), (uchar)(clamp01(colour.z) * 255));
}

/// <summary>
/// Main kernel function. Calculates the colour for a pixel with the specified coordinate position range 0-1.
/// </summary>
/// <param name="screen_coordinate"></param>
/// <param name="colours"></param>
/// <param name="total_number_of_pixels"></param>
/// <param name="time"></param>
/// <param name="camera_position"></param>
/// <param name="camera_facing"></param>
/// <param name="camera_aspect_ratio"></param>
__kernel void calculatePixelColour(
	__global const float2* screen_coordinate, __global uchar* colours, const uint total_number_of_pixels,
	const float time, const float3 camera_position, const float3 camera_facing, const float camera_aspect_ratio)
{
	// Get gloabl thread ID
	int ID = get_global_id(0);
	int output_ID = ID * 4;

	// Make sure we are within the array size
	if (ID < total_number_of_pixels)
	{
		Ray ray = getCameraRay(screen_coordinate[ID], camera_position, camera_facing, camera_aspect_ratio);

		float3 colour = trace(ray, time);

		// Apply gamma correction
		//colour = pow(colour, 0.45f);

		uchar3 colour_8_bit = convertColourTo8Bit(colour);

		colours[output_ID] = colour_8_bit.r;
		colours[output_ID + 1] = colour_8_bit.g;
		colours[output_ID + 2] = colour_8_bit.b;
		colours[output_ID + 3] = 255;
	}
}

#endif



// Documentation for the main Doxygen page

/// @mainpage
///
/// @section section_development_guide Development Guide
/// 
/// Main kernel documentation can be found @ref RealtimeFractalRenderer/kernels here
/// 
