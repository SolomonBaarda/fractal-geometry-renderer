#define CAMERA_POSITIONS_LENGTH 4
#define CAMERA_POSITIONS_ARRAY { (float4)(1005, 1005, 1005, 1), (float4)(1005, 1005, 1025, 11), (float4)(1005, 1005, 1025, 23), (float4)(1055, 965, 975, 31) }

#define CAMERA_FACING_DIRECTIONS_LENGTH 2
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(0, 0, -1)), 12), (float4)(normalise((float3)(-1, 0, 1)), 22) }

#define DO_BENCHMARK
#define BENCHMARK_START_STOP_TIME (float2)(0.0f, 30.0f)
#define CAMERA_DO_LOOP false

#define BENCHMARK

#include "infinite_spheres.cl"



