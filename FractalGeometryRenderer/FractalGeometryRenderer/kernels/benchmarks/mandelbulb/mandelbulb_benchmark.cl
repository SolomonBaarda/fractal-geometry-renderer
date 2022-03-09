#define CAMERA_POSITIONS_LENGTH 3
#define CAMERA_POSITIONS_ARRAY { (float4)(-4.0, -1.5, 0.0, 5), (float4)(-2.0, -1.4, 0.0, 15), (float4)(-0.6, -1.5, 0.0, 25) }

#define CAMERA_FACING_DIRECTIONS_LENGTH 3
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(-0.7, -0.25, 0.0)), 5), (float4)(normalise((float3)(-0.7, -0.3, 0.0)), 15), (float4)(normalise((float3)(-0.7, -0.6, 0.0)), 25) }

#define DO_BENCHMARK
#define BENCHMARK_START_STOP_TIME (float2)(0.0f, 30.0f)
#define CAMERA_DO_LOOP false

#define BENCHMARK

#include "mandelbulb.cl"