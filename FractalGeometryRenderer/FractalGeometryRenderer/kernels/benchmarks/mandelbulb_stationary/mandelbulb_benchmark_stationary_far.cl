#define CAMERA_POSITIONS_LENGTH 2
#define CAMERA_POSITIONS_ARRAY { (float4)(-8.0, -1.5, 0.0, 0), (float4)(-8.0, -1.5, 0.0, 1)}

#define CAMERA_FACING_DIRECTIONS_LENGTH 1
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(-8.0, -1.5, 0.0)), 0) }

#define DO_BENCHMARK
#define BENCHMARK_START_STOP_TIME (float2)(0.0f, 20.0f)
#define CAMERA_DO_LOOP false

#define BENCHMARK

#include "mandelbulb.cl"