#include "structs.h"
#include "utils.h"

#ifndef SDF
#define SDF

namespace SDF
{
	using namespace Utils;

	// https://www.iquilezles.org/www/articles/distfunctions/distfunctions.htm

	static float sphereSDF(const Vector3& point, const Vector3& sphereCentre, float sphereRadius)
	{
		Vector3 relativePosition = sphereCentre - point;
		return relativePosition.length() - sphereRadius;
	}



	//float static boxSDF(const Vector3& point, const Vector3& boxCentre, const Vector3& boxDimensions)
	//{
	//	Vector3 relativePosition = boxCentre - point;
	//	Vector3 q = relativePosition.absolute() - (boxDimensions / 2);
	//	return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
	//}

	//float static planeSDF(const Vector3& point, const Vector3& planeCentre, const Vector3& planeDimensions)
	//{
	//	Vector3 relativePosition = planeCentre - point;
	//	return dot(relativePosition, planeDimensions) + h;
	//}


}





#endif