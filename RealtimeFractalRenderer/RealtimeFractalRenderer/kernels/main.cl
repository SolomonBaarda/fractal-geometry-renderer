
// Simple compute kernel which computes the square of an input array 
__kernel void square(__global float* input, __global float* output, const unsigned int count)
{
	int i = get_global_id(0);

	if (i < count)
		output[i] = input[i] * input[i];
}

//__kernel void calculatePixelColour(__global const float3* position, __global const float3* direction, __global float3* colour)
//{
//	// Get gloabl thread ID
//	int ID = get_global_id(0);
//
//
//	//Get our global thread ID                                
//	int id = get_global_id(0);
//
//	//Make sure we do not go out of bounds                    
//	if (id < n)
//		c[id] = a[id] + b[id];
//
//
//	colour[ID] = (normal + (float3)(1, 1, 1)) * 0.5f;
//	//colour[ID] = colourAndDistance.xyz;
//}
