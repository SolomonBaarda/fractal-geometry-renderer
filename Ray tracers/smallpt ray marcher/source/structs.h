#include <cmath>

#ifndef STRUCTS
#define STRUCTS

class Vector3
{
public:
	double x, y, z;

	Vector3(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z)
	{}

	Vector3 operator+(const Vector3& b) const
	{
		return Vector3(x + b.x, y + b.y, z + b.z);
	}

	Vector3 operator-(const Vector3& b) const
	{
		return Vector3(x - b.x, y - b.y, z - b.z);
	}

	Vector3 operator*(double b) const
	{
		return Vector3(x * b, y * b, z * b);
	}

	Vector3 operator/(double b) const
	{
		return Vector3(x / b, y / b, z / b);
	}

	Vector3 multiply(const Vector3& b) const
	{
		return Vector3(x * b.x, y * b.y, z * b.z);
	}

	Vector3& normalise()
	{
		return *this = *this * (1 / sqrt(x * x + y * y + z * z));
	}

	Vector3 normalised() const
	{
		return *this * (1 / sqrt(x * x + y * y + z * z));
	}

	float dotProduct(const Vector3& b) const
	{
		return x * b.x + y * b.y + z * b.z;
	}

	Vector3 operator%(Vector3& b) const
	{
		return Vector3(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
	}

	float length() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	Vector3 absolute() const
	{
		return Vector3(abs(x), abs(y), abs(z));
	}
};

struct Ray
{
	Vector3 origin, direction;
	Ray(Vector3 origin, Vector3 direction) : origin(origin), direction(direction) {}
};


#endif
