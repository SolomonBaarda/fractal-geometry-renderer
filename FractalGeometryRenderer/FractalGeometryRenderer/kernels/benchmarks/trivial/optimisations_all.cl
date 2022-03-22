// Features
#define DO_AMBIENT_LIGHTING true
#define DO_DIFFUSE_LIGHTING true
#define DO_SPECULAR_HIGHLIGHTS true

#define DO_HARD_SHADOWS false
#define DO_SOFT_SHADOWS true
#define DO_GEOMETRY_GLOW false

// Optimisations
#define INCREASE_INTERSECTION_EPSILON_LINEARLY true

#define BENCHMARK true

#define CAMERA_POSITIONS_LENGTH 2
#define CAMERA_POSITIONS_ARRAY { (float4)(15, -5, 0, 0), (float4)(15, -5, 0, 0) }
#define CAMERA_FACING_DIRECTIONS_LENGTH 1
#define CAMERA_FACING_DIRECTIONS_ARRAY { (float4)(normalise((float3)(1, 0, 0)), 0.0f) }

#define DO_BENCHMARK
#define BENCHMARK_START_STOP_TIME (float2)(0.0f, 30.0f)
#define CAMERA_DO_LOOP false

#include "trivial.cl"