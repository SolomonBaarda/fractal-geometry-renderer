#include "sdf.cl"

float4 sierpinskiTetrahedronSDF(float3 position, int iterations, float bailout)
{
	// http://www.fractalforums.com/sierpinski-gasket/kaleidoscopic-(escape-time-ifs)/

	float r = position.x * position.x + position.y * position.y + position.z * position.z;
	float x = position.x;
	float y = position.y;
	float z = position.z;

	int i;
	for (i = 0; i < iterations && r < bailout; i++) {
		//Folding... These are some of the symmetry planes of the tetrahedron
		if (x + y < 0)
		{
			float x1 = -y;
			y = -x;
			x = x1;
		}
		if (x + z < 0)
		{
			float x1 = -z;
			z = -x;
			x = x1;
		}
		if (y + z < 0)
		{
			float y1 = -z;
			z = -y;
			y = y1;
		}

		//Stretche about the point [1,1,1]*(SCALE-1)/SCALE; The "(SCALE-1)/SCALE" is here in order to keep the size of the fractal constant wrt SCALE
		x = 2.0f * x - 1.0f; //equivalent to: x=SCALE*(x-cx); where cx=(SCALE-1)/SCALE;
		y = 2.0f * y - 1.0f;
		z = 2.0f * z - 1.0f;
		r = x * x + y * y + z * z;
	}

	return (float4)((float3)(0.5f), (sqrt(r) - 2.0f) * pow(2.0f, -i));
}

float4 sierpinskiCubeSDF(float3 position, int iterations)
{
    // https://www.iquilezles.org/www/articles/menger/menger.htm

    float d = boxSDF(position, (float3)(1), (float3)(1));
    float3 res = (float3)(0);

    float s = 1.0f;
    for (int m = 0; m < iterations; m++)
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



