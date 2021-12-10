#pragma once

#include <cmath>

class Vector3
{
public:
	float x, y, z;

	Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z)
	{}

	static Vector3 add(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	static Vector3 subtract(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	static Vector3 multiply(const Vector3& a, float b)
	{
		return Vector3(a.x * b, a.y * b, a.z * b);
	}

	static Vector3 divide(const Vector3& a, float b)
	{
		return Vector3(a.x / b, a.y / b, a.z / b);
	}

	static Vector3 multiplyComponents(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
	}

	static Vector3 divideComponents(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.x / b.x, a.y / b.y, a.z / b.z);
	}

	static Vector3 crossProduct(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
	}

	static float dotProduct(const Vector3& a, const Vector3& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	static Vector3 max(const Vector3& a, const Vector3& b)
	{
		return Vector3(fmax(a.x, b.x), fmax(a.y, b.y), fmax(a.z, b.z));
	}

	static Vector3 min(const Vector3& a, const Vector3& b)
	{
		return Vector3(fmin(a.x, b.x), fmin(a.y, b.y), fmin(a.z, b.z));
	}

	static Vector3 absolute(const Vector3& a)
	{
		return Vector3(abs(a.x), abs(a.y), abs(a.z));
	}

	static Vector3 normalise(const Vector3& a)
	{
		return multiply(a, 1 / sqrt(a.x * a.x + a.y * a.y + a.z * a.z));
	}

	static float squareMagnitude(const Vector3& a)
	{
		return a.x * a.x + a.y * a.y + a.z * a.z;
	}

	static float length(const Vector3& a)
	{
		return sqrt(squareMagnitude(a));
	}

	Vector3 operator+(const Vector3& a) const
	{
		return add(*this, a);
	}

	Vector3 operator-(const Vector3& a) const
	{
		return subtract(*this, a);
	}

	Vector3 operator*(float a) const
	{
		return multiply(*this, a);
	}

	Vector3 operator/(float a) const
	{
		return divide(*this, a);
	}

	Vector3 normalised() const
	{
		return normalise(*this);
	}

	Vector3 normalise()
	{
		return *this = normalised();
	}

	float length() const
	{
		return length(*this);
	}

	Vector3 absolute() const
	{
		return absolute(*this);
	}
};