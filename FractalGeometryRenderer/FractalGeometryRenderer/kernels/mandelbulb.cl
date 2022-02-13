#include "utils.cl"

#define CAMERA_POSITIONS_LENGTH 1
#define CAMERA_POSITIONS_ARRAY { (float4)(-1, -1, -1, 0) }

#define CAMERA_FACING_DIRECTIONS_LENGTH 1
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(-0.5, -0.5, -0.5)), 0.0f) }


#define MAXIMUM_MARCH_STEPS 100
#define MAXIMUM_MARCH_DISTANCE 50.0f

#define SURFACE_INTERSECTION_EPSILON 0.0005f

#define CAMERA_FOCUS_DISTANCE 0.01f

#define DO_GEOMETRY_GLOW

#define SCENE_GLOW_COLOUR (float3)(0.8f, 0.8f, 0.8f)
#define SCENE_BACKGROUND_COLOUR (float3)(0.1f, 0.1f, 0.1f)

#define SCENE_MAX_GLOW_DISTANCE 0.1f


//#define DO_LAMBERTIAN_REFLECTANCE

//#define DO_GAMMA_CORRECTION

//float4 signedDistanceEstimation(float3 position, float time)
//{
//	// http://blog.hvidtfeldts.net/index.php/2011/09/distance-estimated-3d-fractals-v-the-mandelbulb-different-de-approximations/
//
//	float3 pos = position;
//
//	float3 z = pos;
//	float dr = 1.0f;
//	float r = 0.0f;
//
//	for (int i = 0; i < ITERATIONS; i++) {
//		r = magnitude(z);
//
//		if (r > BAILOUT) break;
//
//		// convert to polar coordinates
//		float theta = acos(z.z / r);
//		float phi = atan2(z.y, z.x);
//		dr = pow(r, POWER - 1.0f) * POWER * dr + 1.0f;
//
//		// scale and rotate the point
//		float zr = pow(r, POWER);
//		theta = theta * POWER;
//		phi = phi * POWER;
//
//		// convert back to cartesian coordinates
//		z = zr * (float3)(sin(theta) * cos(phi), sin(phi) * sin(theta), cos(theta));
//		z += pos;
//	}
//
//	return (float4)((float3)(0.5f), 0.5f * log(r) * r / dr);
//}

#define ITERATIONS 10

float4 signedDistanceEstimation(float3 position, float time)
{
	float power = 7.75f + time * 0.01f;

	float3 w = position;
	float m = dot(w, w);
	float4 colorParams = (float4) (absolute(w), m);
	float dz = 1.0f;


	for (int i = 0; i < ITERATIONS; i++)
	{
		dz = 8.0f * pow(sqrt(m), 7.0f) * dz + 1.0f;

		// Calculate power
		float r = length(w);
		float b = power * acos(w.y / r);
		float a = power * atan2(w.x, w.z);
		w = pow(r, power) * (float3) (sin(b) * sin(a), cos(b), sin(b) * cos(a)) + position;

		colorParams = min(colorParams, (float4) (absolute(w), m));
		m = dot(w, w);

		if (m > 256.0f) break;
	}
	//float4 resColor = (float4) (m, colorParams.y, colorParams.z, colorParams.w);

	return (float4)(colorParams.x, colorParams.y, colorParams.z, 0.25f * log(m) * sqrt(m) / dz);
}

#include "main.cl"
