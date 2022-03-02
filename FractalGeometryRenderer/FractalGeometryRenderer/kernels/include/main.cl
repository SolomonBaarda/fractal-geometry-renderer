#ifndef MAIN_CL
/// @cond DOXYGEN_DO_NOT_DOCUMENT
#define MAIN_CL
/// @endcond

#include "defines.cl"
#include "types.cl"

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
	__global bool* do_camera_loop, __global float* camera_speed,
	__global float2* benchmark_start_stop_time)
{
	*camera_up_axis = CAMERA_UP_AXIS;
	*number_camera_positions = CAMERA_POSITIONS_LENGTH;
	*number_camera_facing = CAMERA_FACING_DIRECTIONS_LENGTH;
	*benchmark_start_stop_time = BENCHMARK_START_STOP_TIME;
	*do_camera_loop = CAMERA_DO_LOOP;
	*camera_speed = CAMERA_SPEED;

#if FORCE_FREE_CAMERA
	* number_camera_positions = 1;
	*number_camera_facing = 1;
	*benchmark_start_stop_time = BENCHMARK_START_STOP_TIME_DONT_DO_TIMED;
#endif

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
/// Calculates the surface normal for a any point in the scene.
/// </summary>
/// <param name="position">Position in world space</param>
/// <param name="time">Scene time in seconds</param>
/// <returns>Surface normal vector for the position on the geometry</returns>
float3 estimateSurfaceNormal(const float3 position, const float time)
{
	const float3 xOffset = (float3)(SURFACE_NORMAL_EPSILON, 0, 0);
	const float3 yOffset = (float3)(0, SURFACE_NORMAL_EPSILON, 0);
	const float3 zOffset = (float3)(0, 0, SURFACE_NORMAL_EPSILON);

	float x = signedDistanceEstimation(position + xOffset, time) - signedDistanceEstimation(position - xOffset, time);
	float y = signedDistanceEstimation(position + yOffset, time) - signedDistanceEstimation(position - yOffset, time);
	float z = signedDistanceEstimation(position + zOffset, time) - signedDistanceEstimation(position - zOffset, time);

	return normalize((float3)(x, y, z));
}

/// <summary>
/// Calculates soft shadows for a point on geometry in the scene.
/// </summary>
/// <param name="pointOnGeometry">Position in world space</param>
/// <param name="time">Scene time in seconds</param>
/// <returns>Shadow value</returns>
float calculateSoftShadow(const float3 pointOnGeometry, const float time, const float3 lightPosition)
{
	// https://www.iquilezles.org/www/articles/rmshadows/rmshadows.htm

	float shadow = 1.0f;
	float ph = 1e20;

	float3 distanceVector = lightPosition - pointOnGeometry;
	float3 directionFromGeometryToLight = normalise(distanceVector);
	float distanceFromGeometryToLight = magnitude(distanceVector);

	for (float totalDistance = SURFACE_SHADOW_EPSILON; totalDistance < distanceFromGeometryToLight; )
	{
		float3 currentPosition = pointOnGeometry + directionFromGeometryToLight * totalDistance;
		float distance = signedDistanceEstimation(currentPosition, time);

		// Hit the surface of an object
		// Therefore the original point must be in shadow
		if (distance <= SURFACE_INTERSECTION_EPSILON)
		{
			return 0.0f;
		}

		// Calculate soft shadow values
		float y = distance * distance / (2.0f * ph);
		float d = sqrt(distance * distance - y * y);
		shadow = min(shadow, SURFACE_SHADOW_FALLOFF * d / max(0.0f, totalDistance - y));
		ph = distance;

		totalDistance += distance;
	}

	return shadow;
}

/// <summary>
/// Calculates hard shadows for a point on geometry in the scene.
/// </summary>
/// <param name="pointOnGeometry">Position in world space</param>
/// <param name="time">Scene time in seconds</param>
/// <returns>Shadow value</returns>
float calculateHardShadow(const float3 pointOnGeometry, const float time, const float3 lightPosition)
{
	// https://www.iquilezles.org/www/articles/rmshadows/rmshadows.htm

	float3 distanceVector = lightPosition - pointOnGeometry;
	float3 directionFromGeometryToLight = normalise(distanceVector);
	float distanceFromGeometryToLight = magnitude(distanceVector);

	for (float totalDistance = SURFACE_SHADOW_EPSILON; totalDistance < distanceFromGeometryToLight; )
	{
		float3 currentPosition = pointOnGeometry + directionFromGeometryToLight * totalDistance;
		float distance = signedDistanceEstimation(currentPosition, time);

		// Hit the surface of an object
		// Therefore the original point must be in shadow
		if (distance <= SURFACE_INTERSECTION_EPSILON)
		{
			return 0.0f;
		}

		totalDistance += distance;
	}

	return 1.0f;
}

float3 reflect(float3 incident, float3 normal)
{
	return incident - 2.0f * dot(normal, incident) * normal;
}

/// <summary>
/// Traces the path of a ray.
/// </summary>
/// <param name="ray">A ray</param>
/// <param name="time">Scene time in seconds</param>
/// <returns>The colour that the pixel should be drawn as (range 0-1)</returns>
float3 trace(const Ray ray, const float time)
{
	float totalDistance = 0.0f;
	float closestDistanceToGeometry = FLOAT_MAX_VALUE;
	float3 currentPosition;
	int steps = 0;
	for (; steps < MAXIMUM_MARCH_STEPS && totalDistance < MAXIMUM_MARCH_DISTANCE; steps++)
	{
		currentPosition = ray.position + (ray.direction * totalDistance);
		float distance = signedDistanceEstimation(currentPosition, time);
		totalDistance += distance;

		// Record the closest distance to the geometry
		if (distance < closestDistanceToGeometry)
		{
			closestDistanceToGeometry = distance;
		}

		// Hit the surface of an object
#if INCREASE_INTERSECTION_EPSILON_LINEARLY
		if (distance <= SURFACE_INTERSECTION_EPSILON * totalDistance)
#else
		if (distance <= SURFACE_INTERSECTION_EPSILON)
#endif
		{
			Material material = getMaterial(currentPosition, time);
			Light light = getLight(time);

			float3 ambient = material.ambient;
			float3 diffuse = material.diffuse;
			float3 specular = material.specular;
			float shadow = 1.0f;

			const float3 normal = estimateSurfaceNormal(currentPosition, time);
			const float3 lightDirection = normalise(light.position - currentPosition);

#if DO_RENDER_SURFACE_NORMALS
			// Set ambient and diffuse colours to be surface normal
			ambient = (normal + (float3)(1.0f)) * 0.5f;
			diffuse = ambient;
#endif

			// Ambient
#if DO_AMBIENT_LIGHTING
			ambient *= light.ambient;
#endif

			// Diffuse
#if DO_DIFFUSE_LIGHTING

			diffuse *= light.diffuse * max(dot(normal, lightDirection), 0.0f);
#endif

			// Specular
#if DO_SPECULAR_HIGHLIGHTS

			float3 viewDirection = normalise(ray.position - currentPosition);
			float3 reflectDirection = reflect(-lightDirection, normal);
			specular *= light.specular * pow(max(dot(viewDirection, reflectDirection), 0.0f), material.shininess);
#endif

#if DO_EDGE_SHADING
			shadow = 1 - ((float)(steps) / (float)(MAXIMUM_MARCH_STEPS));
#endif

			// Soft shadows
#if DO_SOFT_SHADOWS
			shadow = calculateSoftShadow(currentPosition, time, light.position);

			// Hard shadows
#elif DO_HARD_SHADOWS
			shadow = calculateHardShadow(currentPosition, time, light.position);
#endif

			return ambient + shadow * (diffuse + specular);
		}
	}

	// Apply a glow colour around the geometry
#if DO_GEOMETRY_GLOW

	if (closestDistanceToGeometry <= SCENE_MAX_GLOW_DISTANCE)
	{
		float glow_strength = 1 - closestDistanceToGeometry / SCENE_MAX_GLOW_DISTANCE;
		return SCENE_BACKGROUND_COLOUR + glow_strength * SCENE_GLOW_COLOUR;
	}

#endif

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
Ray getCameraRay(const float2 screen_coordinate, const float3 camera_position, const float3 camera_facing, const float aspect_ratio)
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
uchar3 convertColourTo8Bit(const float3 colour)
{
	return (uchar3)((uchar)(clamp01(colour.x) * 255), (uchar)(clamp01(colour.y) * 255), (uchar)(clamp01(colour.z) * 255));
}


/// <summary>
/// Main kernel function. Calculates the colour for a pixel with the specified coordinate position range 0-1.
/// </summary>
/// <param name="colours"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="time"></param>
/// <param name="camera_position"></param>
/// <param name="camera_facing"></param>
__attribute__((vec_type_hint(float3)))
__kernel void calculatePixelColour(
	__global uchar* colours, const uint width, const uint height, const float time, const float3 camera_position, const float3 camera_facing)
{
	// Get gloabl thread ID
	const int ID = get_global_id(0);

	// Make sure we are within the array size
	if (ID < width * height)
	{
		// Get x and y pixel coordinates
		int x = ID % width;
		int y = ID / width;
		// Calculate the screen position
		const float2 screen_coordinate = (float2)((float)(x) / (float)(width), (float)(y) / (float)(height));

		const Ray ray = getCameraRay(screen_coordinate, camera_position, camera_facing, (float)(width) / (float)(height));
		float3 colour = trace(ray, time);

#if DO_GAMMA_CORRECTION
		// Apply gamma correction
		colour = pow(colour, GAMMA_CORRECTION_STRENGTH);
#endif

		const uchar3 colour_8_bit = convertColourTo8Bit(colour);

		const int output_ID = ID * 4;
		colours[output_ID] = colour_8_bit.x;
		colours[output_ID + 1] = colour_8_bit.y;
		colours[output_ID + 2] = colour_8_bit.z;
		colours[output_ID + 3] = 255;
	}
}

#endif



// Documentation for the main Doxygen page

/// @mainpage Home
///
/// @section section_development_guide Development Guide
/// A hello world scene can be found here kernels/hello_world.cl. It's recommended to copy and paste 
/// this file and to start creating your scene from that template.
/// 
/// The table below contains a list of the key files that may need to be referenced when creating a scene.
/// 
/// <table>
/// <caption id="multi_row">Key Files</caption>
/// <tr>	<th>File							<th>Description
/// <tr>	<td>kernels/include/defines.cl		<td>Contains all renderer settings
/// <tr>	<td>kernels/include/types.cl		<td>Contains type definitions
/// <tr>	<td>kernels/include/utils.cl		<td>Contains utility methods
/// <tr>	<td>kernels/include/sdf.cl			<td>Contains some common signed distance functions
/// </table>
/// 
/// The table below contains a list of example scenes.
/// 
/// <table>
/// <caption id="multi_row">Scenes</caption>
/// <tr>	<th>File								<th>Description
/// <tr>	<td>kernels/mandelbulb.cl				<td>
/// <tr>	<td>kernels/sierpinski_collection.cl	<td>
/// <tr>	<td>kernels/mandelbrot_zoom.cl			<td>
/// <tr>	<td>kernels/planet.cl					<td>
/// <tr>	<td>kernels/terrain.cl					<td>
/// 
/// </table>

