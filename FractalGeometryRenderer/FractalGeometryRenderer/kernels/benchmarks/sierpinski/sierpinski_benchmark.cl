#define CAMERA_POSITIONS_LENGTH 5
#define CAMERA_POSITIONS_ARRAY {  (float4)(12, -5, 0, 0), (float4)(12, -0.33, -0.33, 2.5), (float4)(3, -0.33, -0.33, 7.5), (float4)(-1, -0.33, -0.33, 20), (float4)(-1, -2, -0.5, 22.5) }

#define CAMERA_FACING_DIRECTIONS_LENGTH 4
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(12, -5, 0)), 0), (float4)(normalise((float3)(1, 0, 0)), 2.5), (float4)(normalise((float3)(1, 0, 0)), 20), (float4)(normalise((float3)(4, -2, -0.5)), 22.5) }

#define BENCHMARK_START_STOP_TIME (float2)(0.0f, 25.0f)
#define DO_BENCHMARK
#define CAMERA_DO_LOOP false

//#define FORCE_FREE_CAMERA true

#define BENCHMARK

#include "sierpinski_collection.cl"