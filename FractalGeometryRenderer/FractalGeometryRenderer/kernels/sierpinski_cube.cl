#define CAMERA_POSITIONS_LENGTH 1
#define CAMERA_POSITIONS_ARRAY { (float4)(-0.4, -0.6, -0.4, 0) }

#define CAMERA_FACING_DIRECTIONS_LENGTH 1
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(-0.5, -0.5, -0.5)), 0.0f) }

#define FORCE_FREE_CAMERA
#define CAMERA_SPEED 1.0f

#include "sdf.cl"

float4 signedDistanceEstimation(float3 position, float time)
{
    // https://www.iquilezles.org/www/articles/menger/menger.htm

    float d = boxSDF(position, (float3)(1), (float3)(1));
    float3 res = (float3)(0);

    float s = 1.0f;
    for (int m = 0; m < 10; m++)
    {
        float3 a = mod(position * s, 2.0f) - 1.0f;
        s *= 3.0f;
        float3 r = absolute(1.0f - 3.0f * absolute(a));

        float da = max(r.x, r.y);
        float db = max(r.y, r.z);
        float dc = max(r.z, r.x);
        float c = (min(da, min(db, dc)) - 1.0f) / s;

        if (c > d)
        {
            d = c;
            res = (float3)(0.2f * da * db * dc, (1.0f + (float)(m)) / 4.0f, 0.0f);
        }
    }

	return (float4)(res, d);
}

#include "main.cl"
