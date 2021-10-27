#donotrun
// (c) 2020 Claude Heiland-Allen
// SPDX-License-Identifier: GPL-3.0-or-later
/*
Vector template instantation.
*/

vec2 add(vec2 a, vec2 b) { return a + b; }

#define VEC Vec2f
#define vec vec2f
#define SCALAR float
#define scalar float
#define VDIM 2
#include "VecBase.frag"
#undef VDIM
#undef scalar
#undef SCALAR
#undef vec
#undef VEC

#define VEC Vec3f
#define vec vec3f
#define SCALAR float
#define scalar float
#define VDIM 3
#include "VecBase.frag"
#undef VDIM
#undef scalar
#undef SCALAR
#undef vec
#undef VEC

#define VEC Vec4f
#define vec vec4f
#define SCALAR float
#define scalar float
#define VDIM 4
#include "VecBase.frag"
#undef VDIM
#undef scalar
#undef SCALAR
#undef vec
#undef VEC

#define VEC Vec2Dual2f
#define vec vec2dual2f
#define SCALAR Dual2f
#define scalar dual2f
#define VDIM 2
#include "VecBase.frag"
#undef VDIM
#undef scalar
#undef SCALAR
#undef vec
#undef VEC

#define VEC Vec3Dual3f
#define vec vec3dual3f
#define SCALAR Dual3f
#define scalar dual3f
#define VDIM 3
#include "VecBase.frag"
#undef VDIM
#undef scalar
#undef SCALAR
#undef vec
#undef VEC

#define VEC Vec4Dual4f
#define vec vec4dual4f
#define SCALAR Dual4f
#define scalar dual4f
#define VDIM 4
#include "VecBase.frag"
#undef VDIM
#undef scalar
#undef SCALAR
#undef vec
#undef VEC

#if __VERSION__ >= 400

#define VEC Vec2fx
#define vec vec2fx
#define SCALAR FloatX
#define scalar floatx
#define VDIM 2
#include "VecBase.frag"
#undef VDIM
#undef scalar
#undef SCALAR
#undef vec
#undef VEC

#define VEC Vec3fx
#define vec vec3fx
#define SCALAR FloatX
#define scalar floatx
#define VDIM 3
#include "VecBase.frag"
#undef VDIM
#undef scalar
#undef SCALAR
#undef vec
#undef VEC

#define VEC Vec4fx
#define vec vec4fx
#define SCALAR FloatX
#define scalar floatx
#define VDIM 4
#include "VecBase.frag"
#undef VDIM
#undef scalar
#undef SCALAR
#undef vec
#undef VEC

#define VEC Vec2Dual2fx
#define vec vec2dual2fx
#define SCALAR Dual2fx
#define scalar dual2fx
#define VDIM 2
#include "VecBase.frag"
#undef VDIM
#undef scalar
#undef SCALAR
#undef vec
#undef VEC

#define VEC Vec3Dual3fx
#define vec vec3dual3fx
#define SCALAR Dual3fx
#define scalar dual3fx
#define VDIM 3
#include "VecBase.frag"
#undef VDIM
#undef scalar
#undef SCALAR
#undef vec
#undef VEC

#define VEC Vec4Dual4fx
#define vec vec4dual4fx
#define SCALAR Dual4fx
#define scalar dual4fx
#define VDIM 4
#include "VecBase.frag"
#undef VDIM
#undef scalar
#undef SCALAR
#undef vec
#undef VEC

dvec2 add(dvec2 a, dvec2 b) { return a + b; }

#define VEC Vec2d
#define vec vec2d
#define SCALAR double
#define scalar double
#define VDIM 2
#include "VecBase.frag"
#undef VDIM
#undef scalar
#undef SCALAR
#undef vec
#undef VEC

#define VEC Vec3d
#define vec vec3d
#define SCALAR double
#define scalar double
#define VDIM 3
#include "VecBase.frag"
#undef VDIM
#undef scalar
#undef SCALAR
#undef vec
#undef VEC

#define VEC Vec4d
#define vec vec4d
#define SCALAR double
#define scalar double
#define VDIM 4
#include "VecBase.frag"
#undef VDIM
#undef scalar
#undef SCALAR
#undef vec
#undef VEC

#define VEC Vec2Dual2d
#define vec vec2dual2d
#define SCALAR Dual2d
#define scalar dual2d
#define VDIM 2
#include "VecBase.frag"
#undef VDIM
#undef scalar
#undef SCALAR
#undef vec
#undef VEC

#define VEC Vec3Dual3d
#define vec vec3dual3d
#define SCALAR Dual3d
#define scalar dual3d
#define VDIM 3
#include "VecBase.frag"
#undef VDIM
#undef scalar
#undef SCALAR
#undef vec
#undef VEC

#define VEC Vec4Dual4d
#define vec vec4dual4d
#define SCALAR Dual4d
#define scalar dual4d
#define VDIM 4
#include "VecBase.frag"
#undef VDIM
#undef scalar
#undef SCALAR
#undef vec
#undef VEC

#define VEC Vec2Compensated2
#define vec vec2compensated2
#define SCALAR Compensated2
#define scalar compensated2
#define VDIM 2
#include "VecBase.frag"
#undef VDIM
#undef scalar
#undef SCALAR
#undef vec
#undef VEC

Vec2Compensated2 vec2compensated2(dvec2 a)
{
  return vec2compensated2(Compensated2[2](compensated2(a[0]), compensated2(a[1])));
}

Vec2Compensated2 add(dvec2 a, Vec2Compensated2 b)
{
  return vec2compensated2(Compensated2[2](add(a.x, b.v[0]), add(a.y, b.v[1])));
}

Vec2Compensated2 add(Vec2Compensated2 b, dvec2 a)
{
  return vec2compensated2(Compensated2[2](add(a.x, b.v[0]), add(a.y, b.v[1])));
}

#endif

Complexf complexf(vec2 z)
{
  return complexf(z[0], z[1]);
}

#if __VERSION__ >= 400

Complexd complexd(dvec2 z)
{
  return complexd(z[0], z[1]);
}

ComplexCompensated2 complexcompensated2(Vec2Compensated2 z)
{
  return complexcompensated2(z.v[0], z.v[1]);
}

#endif
