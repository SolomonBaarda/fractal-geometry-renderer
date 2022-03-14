#define CAMERA_POSITIONS_LENGTH 7
#define CAMERA_POSITIONS_ARRAY {  (float4)(8, 0, -4, 5), (float4)(4, 0, 0, 10), (float4)(2, 0, 2, 15), (float4)(2, 0, 2, 20), (float4)(1, -0.3, 1, 25), (float4)(-2.65, -0.65, -1.45, 30), (float4)(-2.65, -0.65, -4.55, 40)}

#define CAMERA_FACING_DIRECTIONS_LENGTH 6
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(0.99, 0.09, -0.12)), 3), (float4)(normalise((float3)(1, 0, -1)), 5), (float4)(normalise((float3)(1, 0, -1)), 15), (float4)(normalise((float3)(1, 0.3, 1)), 20), (float4)(normalise((float3)(1, 0.3, 1)), 27.5), (float4)(normalise((float3)(0, 0, 1)), 30) }

#define BENCHMARK_START_STOP_TIME (float2)(0.0f, 40.0f)
#define DO_BENCHMARK
#define BENCHMARK_START_STOP_TIME (float2)(0.0f, 40.0f)
#define CAMERA_DO_LOOP false

#define BENCHMARK

#include "sierpinski_collection.cl"