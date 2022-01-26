#include "utils.cl"

#define CAMERA_POSITIONS_LENGTH 1
#define CAMERA_POSITIONS_ARRAY { (float4)(-0.5, -0.5, -0.5, 0) }

#define CAMERA_FACING_DIRECTIONS_LENGTH 1
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(-0.5, -0.5, -0.5)), 0.0f) }

#define ITERATIONS 50
#define BAILOUT 100000000
#define POWER 8.0f

#define MAXIMUM_MARCH_STEPS 1000
#define MAXIMUM_MARCH_DISTANCE 100000000.0f

#define CAMERA_FOCUS_DISTANCE 0.1f



float4 signedDistanceEstimation(float3 position, float time)
{
	float3 pos = position;

	float3 z = pos;
	float dr = 1.0f;
	float r = 0.0f;

	for (int i = 0; i < ITERATIONS; i++) {
		r = magnitude(z);

		if (r > BAILOUT) break;

		// convert to polar coordinates
		float theta = acos(z.z / r);
		float phi = atan2(z.y, z.x);
		dr = pow(r, POWER - 1.0f) * POWER * dr + 1.0f;

		// scale and rotate the point
		float zr = pow(r, POWER);
		theta = theta * POWER;
		phi = phi * POWER;

		// convert back to cartesian coordinates
		z = zr * (float3)(sin(theta) * cos(phi), sin(phi) * sin(theta), cos(theta));
		z += pos;
	}

	return (float4)((float3)(0.5f), 0.5f * log(r) * r / dr);
}

#include "main.cl"
