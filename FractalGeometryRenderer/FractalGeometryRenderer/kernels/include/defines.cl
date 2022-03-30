#ifndef DEFINES_CL
/// @cond DOXYGEN_DO_NOT_DOCUMENT
#define DEFINES_CL
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

#ifndef FLOAT_MAX_VALUE 
#define FLOAT_MAX_VALUE 3e38
#endif

#ifndef SURFACE_INTERSECTION_EPSILON
/// <summary>Epsilon value used to define the maximum distance which is considered a ray collision</summary>
/// <returns>float</returns>
#define SURFACE_INTERSECTION_EPSILON 0.001f
#endif


#ifndef LINEAR_INTERSECTION_EPSILON_MULTIPLIER
/// <summary></summary>
/// <returns>float</returns>
#define LINEAR_INTERSECTION_EPSILON_MULTIPLIER 0.75f
#endif

#ifndef INCREASE_INTERSECTION_EPSILON_LINEARLY
/// <summary></summary>
/// <returns>bool</returns>
#define INCREASE_INTERSECTION_EPSILON_LINEARLY true
#endif

#ifndef SURFACE_NORMAL_EPSILON
/// <summary>Epsilon value used to calculate surface the normal of geometry</summary>
/// <returns>float</returns>
#define SURFACE_NORMAL_EPSILON 0.001f
#endif



#ifndef USE_BOUNDING_VOLUME
/// <summary></summary>
/// <returns>bool</returns>
#define USE_BOUNDING_VOLUME false
#endif

#ifndef DISPLAY_BOUNDING_VOLUME
/// <summary></summary>
/// <returns>bool</returns>
#define DISPLAY_BOUNDING_VOLUME false
#endif



#if DISPLAY_BOUNDING_VOLUME && ! USE_BOUNDING_VOLUME
#error "Cannot display bounding volumes when they are not enabled"
#endif






#ifndef BOUNDING_VOLUME_INTERSECTION_EPSILON
/// <summary></summary>
/// <returns>bool</returns>
#define BOUNDING_VOLUME_INTERSECTION_EPSILON 0.1f
#endif

#ifndef BOUNDING_VOLUME_COLOUR
/// <summary></summary>
/// <returns>float3</returns>
#define BOUNDING_VOLUME_COLOUR (float3)(0.4f, 0.1f, 0.1f)
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

#ifndef CAMERA_SPEED
/// <summary></summary>
/// <returns></returns>
#define CAMERA_SPEED 5.0f
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

/// <summary>Epsilon value used when calculating shadows on geometry. Represents the distance at which 
/// shadow checks start being made. Must be greater than SURFACE_INTERSECTION_EPSILON to prevent artifacting</summary>
/// <returns>float</returns>
#ifndef SURFACE_SHADOW_EPSILON
#define SURFACE_SHADOW_EPSILON 0.1f
#endif

/// <summary>Falloff value used when calculating soft shadows</summary>
/// <returns>float</returns>
#ifndef SURFACE_SHADOW_FALLOFF
#define SURFACE_SHADOW_FALLOFF 5.0f
#endif



#ifndef SCENE_GLOW_COLOUR
#define SCENE_GLOW_COLOUR (float3)(1)
#endif

#ifndef SCENE_MAX_GLOW_DISTANCE
#define SCENE_MAX_GLOW_DISTANCE 10.0f
#endif



#ifndef GAMMA_CORRECTION_STRENGTH
#define GAMMA_CORRECTION_STRENGTH 0.45f
#endif

#ifndef DO_GAMMA_CORRECTION
/// <summary></summary>
/// <returns>bool</returns>
#define DO_GAMMA_CORRECTION false
#endif


#ifndef FORCE_FREE_CAMERA
/// <summary></summary>
/// <returns>bool</returns>
#define FORCE_FREE_CAMERA false

#else
// DO_BENCHMARK is defined

//#ifndef BENCHMARK_START_STOP_TIME
//#error "BENCHMARK_START_STOP_TIME must be defined"
//#endif
//
//#ifndef BENCHMARK_START_STOP_TIME
//#error "BENCHMARK_START_STOP_TIME must be defined"
//#endif

#endif

#define BENCHMARK_START_STOP_TIME_DONT_DO_TIMED (float2)(-1, -1)


#ifndef BENCHMARK_START_STOP_TIME
/// <summary>The start and stop time that the benchmarker should use. If the values are negative, then 
/// the benchmarker will run for the entire durarion that the scene is active</summary>
/// <returns>float2</returns>
#define BENCHMARK_START_STOP_TIME BENCHMARK_START_STOP_TIME_DONT_DO_TIMED
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





#ifndef DO_RENDER_SURFACE_NORMALS
/// <summary></summary>
/// <returns>bool</returns>
#define DO_RENDER_SURFACE_NORMALS false
#endif


#ifndef DO_RENDER_MARCHING_ITERATIONS
/// <summary></summary>
/// <returns>bool</returns>
#define DO_RENDER_MARCHING_ITERATIONS false
#endif






#ifndef DO_SOFT_SHADOWS
/// <summary></summary>
/// <returns></returns>
#define DO_SOFT_SHADOWS false
#endif

#ifndef DO_HARD_SHADOWS
/// <summary></summary>
/// <returns></returns>
#define DO_HARD_SHADOWS false
#endif

#if DO_SOFT_SHADOWS && DO_HARD_SHADOWS
#error "DO_SOFT_SHADOWS and DO_HARD_SHADOWS cannot be used at the same time"
#endif


#ifndef DO_AMBIENT_LIGHTING
/// <summary></summary>
/// <returns></returns>
#define DO_AMBIENT_LIGHTING true
#endif

#ifndef DO_DIFFUSE_LIGHTING
/// <summary></summary>
/// <returns></returns>
#define DO_DIFFUSE_LIGHTING true
#endif

#ifndef DO_SPECULAR_HIGHLIGHTS
/// <summary></summary>
/// <returns></returns>
#define DO_SPECULAR_HIGHLIGHTS true
#endif

#ifndef DO_GEOMETRY_GLOW
/// <summary></summary>
/// <returns></returns>
#define DO_GEOMETRY_GLOW false
#endif






#endif

