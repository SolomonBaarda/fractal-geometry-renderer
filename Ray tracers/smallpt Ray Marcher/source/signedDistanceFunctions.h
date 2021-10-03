#include "structs.h"
#include "utils.h"

#ifndef SDF
#define SDF

namespace SDF
{
	// https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm

	float static sphereSDF(const Vector3& point, const Vector3& sphereCentre, float sphereRadius)
	{
		return (sphereCentre - point).length() - sphereRadius;
	}



	//float static boxSDF(const Vector3& p, const Vector3& b)
	//{
	//	Vector3 q = p  - b;
	//	return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
	//}
}





#endif