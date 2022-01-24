#ifndef UTILS_CL
#define UTILS_CL

#ifndef PI
#define PI 3.1415926535897932385f
#endif

/// <summary>
/// </summary>
/// <param name=""></param>
float f_abs(float value)
{
	return value < 0 ? -1 * value : value;
}

/// <summary>
/// </summary>
/// <param name=""></param>
float3 absolute(float3 value)
{
	return (float3)(
		value.x < 0 ? -1 * value.x : value.x,
		value.y < 0 ? -1 * value.y : value.y,
		value.z < 0 ? -1 * value.z : value.z
		);
}

/// <summary>
/// </summary>
/// <param name=""></param>
float magnitude(float3 vec)
{
	return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

/// <summary>
/// </summary>
/// <param name=""></param>
float clamp01(float a)
{
	return a < 0 ? 0 : a > 1 ? 1 : a;
}

/// <summary>
/// </summary>
/// <param name=""></param>
float degreesToRadians(float degrees) {
	return degrees * PI / 180.0f;
}

/// <summary>
/// </summary>
/// <param name=""></param>
float3 normalise(float3 vec)
{
	return vec / sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

/// <summary>
/// </summary>
/// <param name=""></param>
float3 crossProduct(float3 a, float3 b)
{
	return (float3)(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

/// <summary>
/// </summary>
/// <param name=""></param>
float dotProduct(float3 a, float3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

/// <summary>
/// </summary>
/// <param name=""></param>
float3 mod(float3 a, float3 b)
{
	return (float3)(fmod(a.x, b.x), fmod(a.y, b.y), fmod(a.z, b.z));
}

#endif