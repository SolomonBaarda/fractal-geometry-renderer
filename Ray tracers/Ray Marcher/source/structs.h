#include <cmath>

#ifndef STRUCTS
#define STRUCTS

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

	/// <summary>
	/// Component wise multiplication
	/// </summary>
	/// <param name="b"></param>
	/// <returns></returns>
	Vector3 operator*(const Vector3& b) const
	{
		return Vector3(x * b.x, y * b.y, z * b.z);
	}

	Vector3 operator/(float b) const
	{
		return Vector3(x / b, y / b, z / b);
	}

	Vector3 multiply(const Vector3& b) const
	{
		return Vector3(x * b.x, y * b.y, z * b.z);
	}

	Vector3 normalised() const
	{
		return *this * (1 / sqrt(x * x + y * y + z * z));
	}

	Vector3 normalise()
	{
		return *this = normalised();
	}

	static Vector3 crossProduct(const Vector3& u, const Vector3& v) {
		return Vector3(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
	}

	static float dotProduct(const Vector3& a, const Vector3& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	float dotProduct(const Vector3& b) const
	{
		return dotProduct(*this, b);
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

class Ray {
public:
	Vector3 origin;
	Vector3 direction;
	double distance;

	Ray() {}
	Ray(const Vector3& origin, const Vector3& direction)
		: origin(origin), direction(direction), distance(0)
	{}

	Ray(const Vector3& origin, const Vector3& direction, double distance)
		: origin(origin), direction(direction), distance(distance)
	{}

	Vector3 at(double t) const {
		return origin + direction * t;
	}

	Vector3 at() const {
		return at(distance);
	}
};



#endif
