#define CAMERA_POSITIONS_LENGTH 1
#define CAMERA_POSITIONS_ARRAY { (float4)(0, -10, 0, 0) }

#define CAMERA_FACING_DIRECTIONS_LENGTH 1
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(-10, -10, -10)), 0.0f) }

#define SCENE_LIGHT_POSITION (float3)(100, -100, 100)
#define SCENE_LIGHT_COLOUR (float3)(1.0f, 1.0f, 0.98f)

#define SCENE_BACKGROUND_COLOUR (float3)(0.5f, 0.5f, 0.9f)

#define MAXIMUM_MARCH_STEPS 200
#define MAXIMUM_MARCH_DISTANCE 300.0f


#define CAMERA_FOCUS_DISTANCE 0.01f



//#define RENDER_NORMALS
//#define DO_LIGHTING

#include "sdf.cl"
#include "simplexnoise1234.c"


#define SCALE 0.025f
#define AMPLITUDE 10.0f


float getHeightAt(float x, float y)
{
	return snoise2(x * SCALE, y * SCALE) * AMPLITUDE;
}

float4 signedDistanceEstimation(float3 position, float time)
{
	// https://fileadmin.cs.lth.se/cs/Education/EDAN35/projects/16NiklasJohan_Terrain.pdf

	float height = getHeightAt(position.x, position.z);

	float distance = 0;

	if (position.y < height)
	{
		distance = f_abs(position.y + height);
	}

	return (float4)((float3)(0.5f, 0.5f, 0.5f), distance);
}

#include "main.cl"
