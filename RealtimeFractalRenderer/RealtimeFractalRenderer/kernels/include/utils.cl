#ifndef UTILS_CL
#define UTILS_CL

#ifndef PI
#define PI 3.1415926535897932385f
#endif

float3 absolute(float3 value)
{
	return (float3)(
		value.x < 0 ? -1 * value.x : value.x,
		value.y < 0 ? -1 * value.y : value.y,
		value.z < 0 ? -1 * value.z : value.z
		);
}

float magnitude(float3 vec)
{
	return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

float sphereSDF(float3 position, float3 centre, float radius)
{
	return magnitude(centre - position) - radius;
}

float boxSDF(float3 position, float3 centre, float3 dimensions)
{
	float3 q = absolute(centre - position) - dimensions;
	float length = magnitude((float3)(max(q.x, 0.0f), max(q.y, 0.0f), max(q.z, 0.0f)));
	return length + min(max(q.x, max(q.y, q.z)), 0.0f);
}

float clamp01(float a)
{
	return a < 0 ? 0 : a > 1 ? 1 : a;
}

float degreesToRadians(float degrees) {
	return degrees * PI / 180.0f;
}

float3 normalise(float3 vec)
{
	return vec / sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

float3 crossProduct(float3 a, float3 b)
{
	return (float3)(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

float3 mod(float3 a, float3 b)
{
	return (float3)(fmod(a.x, b.x), fmod(a.y, b.y), fmod(a.z, b.z));
}

#endif