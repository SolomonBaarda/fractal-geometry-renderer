#ifndef UTILS_CL
/// @cond DOXYGEN_DO_NOT_DOCUMENT
#define UTILS_CL

#ifndef PI
#define PI 3.1415926535897932385f
#endif

float f_abs(const float value)
{
	return value < 0 ? -1 * value : value;
}

float3 absolute(const float3 value)
{
	return (float3)(
		value.x < 0 ? -1 * value.x : value.x,
		value.y < 0 ? -1 * value.y : value.y,
		value.z < 0 ? -1 * value.z : value.z
		);
}

float magnitude(const float3 vec)
{
	return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

float clamp01(const float a)
{
	return a < 0 ? 0 : a > 1 ? 1 : a;
}

float3 normalise(const float3 vec)
{
	return vec / sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

float3 crossProduct(const float3 a, const float3 b)
{
	return (float3)(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

float dotProduct(const float3 a, const float3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

float3 mod(const float3 a, const float3 b)
{
	return (float3)(fmod(a.x, b.x), fmod(a.y, b.y), fmod(a.z, b.z));
}

float lerp(const float min, const float max, const float f)
{
	return min + f * (max - min);
}

#endif