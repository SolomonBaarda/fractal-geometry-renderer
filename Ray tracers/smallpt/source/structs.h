struct Vector3
{                 // Usage: time ./smallpt 5000 && xv image.ppm
	double x, y, z; // position, also color (r,g,b)

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

	Vector3 multiply(const Vector3& b) const
	{
		return Vector3(x * b.x, y * b.y, z * b.z);
	}

	Vector3& normalise()
	{
		return *this = *this * (1 / sqrt(x * x + y * y + z * z));
	}

	double dotProduct(const Vector3& b) const
	{
		return x * b.x + y * b.y + z * b.z;
	}

	Vector3 operator%(Vector3& b)
	{
		return Vector3(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
	}
};

struct Ray
{
	Vector3 origin, direction;
	Ray(Vector3 origin, Vector3 direction) : origin(origin), direction(direction) {}
};

/// <summary>
/// Material types, used in radiance()
/// </summary>
enum class ReflectionType
{
	DIFFUSE,
	SPECULAR,
	REFRACTIVE
};

struct Sphere
{
	double radius;
	Vector3 centre, emission, colour;
	ReflectionType reflection;

	Sphere(double radius, Vector3 centre, Vector3 emission, Vector3 colour, ReflectionType reflection) :
		radius(radius), centre(centre), emission(emission), colour(colour), reflection(reflection)
	{}

	/// <summary>
	/// </summary>
	/// <param name="r"></param>
	/// <returns>distance, 0 if no hit</returns>
	double intersect(const Ray& r) const
	{
		// Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
		Vector3 op = centre - r.origin; 
		double t, eps = 1e-4, b = op.dotProduct(r.direction), det = b * b - op.dotProduct(op) + radius * radius;
		if (det < 0)
			return 0;
		else
			det = sqrt(det);
		return (t = b - det) > eps ? t : ((t = b + det) > eps ? t : 0);
	}
};

