#include "utils.cl"

#ifndef DISABLE_FREE_CAMERA_MANDELBROT
#define DISABLE_FREE_CAMERA_MANDELBROT true
#endif

#if DISABLE_FREE_CAMERA_MANDELBROT

#define CAMERA_POSITIONS_LENGTH 1
#define CAMERA_POSITIONS_ARRAY { (float4)(0, -1.5, 0, 0) }
#define CAMERA_FACING_DIRECTIONS_LENGTH 1
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(-0.7, -0.3, -0.7)), 0) }
#define CAMERA_SPEED 0.1f

#endif

#define MAXIMUM_MARCH_STEPS 100
#define MAXIMUM_MARCH_DISTANCE 50.0f
#define SURFACE_INTERSECTION_EPSILON 0.001f
#define CAMERA_FOCUS_DISTANCE 0.0001f
#define SCENE_BACKGROUND_COLOUR (float3)(0.1f, 0.1f, 0.1f)

#define DO_AMBIENT_LIGHTING true
#define DO_DIFFUSE_LIGHTING false
#define DO_SPECULAR_HIGHLIGHTS false

#include "types.cl"
#include "sdf.cl"



#ifndef ITERATIONS
#define ITERATIONS 500
#endif 

#ifndef ESCAPE
#define ESCAPE 4
#endif

#ifndef SCALE
#define SCALE 0.5f
#endif

float3 getMandelbrot(float2 position)
{
	// http://www.fractalforums.com/3d-fractal-generation/true-3d-mandlebrot-type-fractal/msg8505/#msg8505

	float x_temp;
	float dx_temp;
	float r, dr, dist;
	float escape;

	//Initialize iteration variables
	float x0 = position.x;
	float y0 = position.y;
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

	return colour;
}



Light getLight(float time)
{
    Light light;
    light.ambient = (float3)(1.0f, 1.0f, 1.0f);
    light.diffuse = (float3)(0);
    light.specular = (float3)(0);
    light.position = (float3)(0, -1, 0);

    return light;
}

#if DISABLE_FREE_CAMERA_MANDELBROT

float2 getSamplePoint(float x, float y, float time)
{
    return (float2)(x, y) * SCALE;
}

#endif

Material getMaterial(float3 position, float time)
{
    // Material
    Material material;
    material.ambient = getMandelbrot(getSamplePoint(position.x, position.z, time));
    material.diffuse = material.ambient;
    material.specular = (float3)(0.5f, 0.5f, 0.5f);
    material.shininess = 25.0f;

    return material;
}

float DE(float3 position, float time)
{
    // Distance estimation
    return f_abs(position.y);
}

#include "main.cl"
