#include "utils.cl"

#define CAMERA_POSITIONS_LENGTH 2
#define CAMERA_POSITIONS_ARRAY { (float4)(0, -5, 0, 0), (float4)(0, -5, 0, 10) }

#define CAMERA_FACING_DIRECTIONS_LENGTH 1
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(0, -1, 0)), 0) }

#define DISABLE_FREE_CAMERA_MANDELBROT false

float2 getSamplePoint(float x, float y, float time)
{
	float scale = pow(2.0f, time * 0.4f);
	return (float2)(-0.26559 + x / scale, -0.65065 + y / scale);
}

#include "mandelbrot.cl"

