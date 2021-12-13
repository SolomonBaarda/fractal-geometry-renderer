
#ifndef __OPENCL_C_VERSION__
#include <cmath>
#endif

__kernel void calculatePixelColour(__global const float3* position, __global const float3* direction, 
	__global float3* colour, const unsigned int max_length)
{
	// Get gloabl thread ID
	int ID = get_global_id(0);

	// Make sure we are within the array size
	if (ID < max_length)
	{
		colour[ID] = (float3)(1);
	}
}
