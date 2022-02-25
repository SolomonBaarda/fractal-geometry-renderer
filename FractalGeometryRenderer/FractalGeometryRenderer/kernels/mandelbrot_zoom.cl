#include "utils.cl"

#define CAMERA_POSITIONS_LENGTH 2
#define CAMERA_POSITIONS_ARRAY { (float4)(0, -5, 0, 0), (float4)(0, -5, 0, 10) }

#define CAMERA_FACING_DIRECTIONS_LENGTH 1
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(0, -1, 0)), 0) }

#define MAXIMUM_MARCH_STEPS 100
#define MAXIMUM_MARCH_DISTANCE 50.0f

#define SURFACE_INTERSECTION_EPSILON 0.001f

#define CAMERA_FOCUS_DISTANCE 0.0001f


#define SCENE_BACKGROUND_COLOUR (float3)(0.1f, 0.1f, 0.1f)


#define CAMERA_SPEED 0.25f

#define SCENE_LIGHT_POSITION (float3)(0, -5, -5)
#define SCENE_LIGHT_COLOUR (float3)(2.0f, 2.0f, 2.0f)

#define DO_LAMBERTIAN_REFLECTANCE
//#define DO_SOFT_SHADOWS

//#define DO_GAMMA_CORRECTION

#define ITERATIONS 1000
#define ESCAPE 4




float4 signedDistanceEstimation(float3 position, float time)
{
	// http://www.fractalforums.com/3d-fractal-generation/true-3d-mandlebrot-type-fractal/msg8505/#msg8505

	float x_temp;
	float dx_temp;
	float r, dr, dist;
	float escape;

	float scale = pow(2.0f, time * 0.4f);
	
	float x0 = -0.26559 + position.x / scale;
	float y0 = -0.65065 + position.z / scale;
	float x = 0.0f;
	float y = 0.0f;
	float dx = 0.0f;
	float dy = 0.0f;

	int i;
	for (i = 0; i < ITERATIONS && x * x + y * y < ESCAPE; i++)
	{
		//Update z'
		dx_temp = 2.0f * (x * dx - y * dy) + 1.0f;
		dy = 2.0f * (x * dy + y * dx);
		dx = dx_temp;

		//Update z
		x_temp = x * x - y * y + x0;
		y = 2.0f * x * y + y0;
		x = x_temp;
	}

	// Calculate distance
	//r = sqrt(x * x + y * y);
	//dr = sqrt(dx * dx + dy * dy);
	//dist = 0.5f * r * log(r) / dr;

	float3 colour;
	// Outside the mandelbrot
	if (i < ITERATIONS)
	{
		colour = (float3)(0.75f, 0.0f, 0.0f);
	}
	// Inside the mandelbrot
	else
	{
		colour = (float3)(0);
	}

	return (float4)(colour, f_abs(position.y));
}

#include "main.cl"
