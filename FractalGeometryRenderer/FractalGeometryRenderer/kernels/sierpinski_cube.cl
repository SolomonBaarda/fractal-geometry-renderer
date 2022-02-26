#define CAMERA_POSITIONS_LENGTH 1
#define CAMERA_POSITIONS_ARRAY { (float4)(-0.4, -0.6, -0.4, 0) }

#define CAMERA_FACING_DIRECTIONS_LENGTH 1
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(-0.5, -0.5, -0.5)), 0.0f) }

#define MAXIMUM_MARCH_STEPS 100
#define MAXIMUM_MARCH_DISTANCE 10.0f

#define SURFACE_INTERSECTION_EPSILON 0.000001f

#define FORCE_FREE_CAMERA
#define CAMERA_SPEED 1.0f

#include "sierpinski.cl"

float4 signedDistanceEstimation(float3 position, float time)
{
	float4 colAndDist = sierpinskiCubeSDF(position, 10);

	return (float4)(colAndDist);
}

#include "main.cl"
