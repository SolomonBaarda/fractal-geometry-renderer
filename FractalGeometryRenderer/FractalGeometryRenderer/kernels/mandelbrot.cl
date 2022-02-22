#include "utils.cl"

#define CAMERA_POSITIONS_LENGTH 1
#define CAMERA_POSITIONS_ARRAY { (float4)(0, -1.5, 0, 0) }

#define CAMERA_FACING_DIRECTIONS_LENGTH 1
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(-0.7, -0.3, -0.7)), 0) }

#define MAXIMUM_MARCH_STEPS 100
#define MAXIMUM_MARCH_DISTANCE 50.0f

#define SURFACE_INTERSECTION_EPSILON 0.001f

#define CAMERA_FOCUS_DISTANCE 0.01f


#define SCENE_BACKGROUND_COLOUR (float3)(0.1f, 0.1f, 0.1f)


#define CAMERA_SPEED 0.25f

#define SCENE_LIGHT_POSITION (float3)(0, -5, -5)
#define SCENE_LIGHT_COLOUR (float3)(2.0f, 2.0f, 2.0f)

#define DO_LAMBERTIAN_REFLECTANCE
//#define DO_SOFT_SHADOWS

//#define DO_GAMMA_CORRECTION

#define ITERATIONS 100




float4 signedDistanceEstimation(float3 position, float time)
{
	float x0 = position.x;
	float y0 = position.z;
	float x = 0.0f;
	float y = 0.0f;
	//float x2 = 0.0f;
	//float y2 = 0.0f;

	int i;
	for (i = 0; i < ITERATIONS && x * x + y * y <= 4; i++)
	{
		//y = 2 * x * y + y0;
		//x = x2 - y2 + x0;
		//x2 = x * x;
		//y2 = y * y;
		float temp = x * x - y * y + x0;
		y = 2 * x * y + y0;
		x = temp;
	}

	float colour = (float)(i) / (float)(ITERATIONS);

	return (float4)(i, i, i, f_abs(position.y));
}

#include "main.cl"
