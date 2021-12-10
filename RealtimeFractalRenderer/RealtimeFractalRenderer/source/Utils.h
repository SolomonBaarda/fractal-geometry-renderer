#pragma once

const double PI = 3.1415926535897932385;

inline float degreesToRadians(float degrees) {
	return degrees * PI / 180.0;
}

inline float clamp(float x, float min, float max)
{
	return x < min ? min : x > max ? max : x;
}

inline float clamp01(float x)
{
	return clamp(x, 0, 1);
}

inline float max(float a, float b)
{
	return a > b ? a : b;
}

inline float min(float a, float b)
{
	return a < b ? a : b;
}

