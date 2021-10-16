#pragma once

#include <cmath>

class Vector3
{
public:
	float x, y, z;

	Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z)
	{}

	Vector3 operator+(const Vector3& b) const
	{
		return Vector3(x + b.x, y + b.y, z + b.z);
	}

	Vector3 operator-(const Vector3& b) const
	{
		return Vector3(x - b.x, y - b.y, z - b.z);
	}

	Vector3 operator*(float b) const
	{
		return Vector3(x * b, y * b, z * b);
	}

	Vector3 operator/(float b) const
	{
		return Vector3(x / b, y / b, z / b);
	}

	/// <summary>
	/// Normalise this vector and return the result.
	/// </summary>
	/// <returns>Normalised vector</returns>
	Vector3& normalise()
	{
		return *this = *this * (1 / sqrt(x * x + y * y + z * z));
	}

	float dotProduct(const Vector3& b) const
	{
		return x * b.x + y * b.y + z * b.z;
	}

	float magnitude() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	Vector3 absolute() const
	{
		return Vector3(abs(x), abs(y), abs(z));
	}




	Vector3 operator%(Vector3& b)
	{
		return Vector3(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
	}
};
