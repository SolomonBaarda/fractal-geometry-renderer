#define CAMERA_POSITIONS_LENGTH 1
#define CAMERA_POSITIONS_ARRAY { (float4)(-0.4, -0.6, -0.4, 0) }

#define CAMERA_FACING_DIRECTIONS_LENGTH 1
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(-0.5, -0.5, -0.5)), 0.0f) }

#include "sdf.cl"

#define SCALE 2.0f
#define BAILOUT 1000

float4 signedDistanceEstimation(float3 position, float time)
{
	// http://www.fractalforums.com/sierpinski-gasket/kaleidoscopic-(escape-time-ifs)/

	float r = position.x * position.x + position.y * position.y + position.z * position.z;
	float x = position.x;
	float y = position.y;
	float z = position.z;

	int i;
	for (i = 0; i < 10 && r < BAILOUT; i++) {
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
		x = SCALE * x - (SCALE - 1); //equivalent to: x=SCALE*(x-cx); where cx=(SCALE-1)/SCALE;
		y = SCALE * y - (SCALE - 1);
		z = SCALE * z - (SCALE - 1);
		r = x * x + y * y + z * z;
	}

	return (float4)((float3)(0.5f), (sqrt(r) - 2.0f) * pow(SCALE, -i));
}

#include "main.cl"
