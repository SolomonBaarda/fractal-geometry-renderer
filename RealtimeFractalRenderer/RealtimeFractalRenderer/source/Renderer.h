#pragma once

#include "Camera.h"

#include <cstdint>
#include <string>
#include <vector>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

class Renderer
{
public:
	Renderer();
	Renderer(uint32_t width, uint32_t height);
	~Renderer();

	void render(const Camera &camera, float time);

	uint8_t* buffer;

	int load_kernel(std::string path);

private:
	uint32_t width, height, size;

	cl_float2* screen_coordinates;
	cl_mem screen_coordinate_input;

	cl_mem colours_output;


	size_t global;                      // global domain size for our calculation
	size_t local;                       // local domain size for our calculation

	cl_platform_id platform;
	cl_device_id device_id;             // compute device id 
	cl_context context;                 // compute context
	cl_command_queue commands;          // compute command queue
	cl_program program;                 // compute program
	cl_kernel kernel;                   // compute kernel




	int setup();
	void cleanup();







	//float signedDistanceEstimation(const Vector3& point, Vector3& outputColour = Vector3())
	//{
	//	//		Vector3 w = point;
	//	//		float m = Vector3::dotProduct(w, w);
	//	//
	//	//		Vector3 trapW = w.absolute();
	//	//		float trapM = abs(m);
	//	//		float dz = 1.0;
	//	//
	//	//		for (int i = 0; i < 4; i++)
	//	//		{
	//	//#if 0
	//	//			// polynomial version (no trigonometrics, but MUCH slower)
	//	//			float m2 = m * m;
	//	//			float m4 = m2 * m2;
	//	//			dz = 8.0 * sqrt(m4 * m2 * m) * dz + 1.0;
	//	//
	//	//			float x = w.x; float x2 = x * x; float x4 = x2 * x2;
	//	//			float y = w.y; float y2 = y * y; float y4 = y2 * y2;
	//	//			float z = w.z; float z2 = z * z; float z4 = z2 * z2;
	//	//
	//	//			float k3 = x2 + z2;
	//	//			float k2 = inversesqrt(k3 * k3 * k3 * k3 * k3 * k3 * k3);
	//	//			float k1 = x4 + y4 + z4 - 6.0 * y2 * z2 - 6.0 * x2 * y2 + 2.0 * z2 * x2;
	//	//			float k4 = x2 - y2 + z2;
	//	//
	//	//			w.x = p.x + 64.0 * x * y * z * (x2 - z2) * k4 * (x4 - 6.0 * x2 * z2 + z4) * k1 * k2;
	//	//			w.y = p.y + -16.0 * y2 * k3 * k4 * k4 + k1 * k1;
	//	//			w.z = p.z + -8.0 * y * k4 * (x4 * x4 - 28.0 * x4 * x2 * z2 + 70.0 * x4 * z4 - 28.0 * x2 * z2 * z4 + z4 * z4) * k1 * k2;
	//	//#else
	//	//			// trigonometric version (MUCH faster than polynomial)
	//	//
	//	//			// dz = 8*z^7*dz
	//	//			dz = 8.0 * pow(m, 3.5) * dz + 1.0;
	//	//			//dz = 8.0*pow(sqrt(m),7.0)*dz + 1.0;
	//	//
	//	//			  // z = z^8+z
	//	//			float r = w.length();
	//	//			float b = 8.0 * acos(w.y / r);
	//	//			float a = 8.0 * atan2(w.x, w.z);
	//	//			w = Vector3(sin(b) * sin(a), cos(b), sin(b) * cos(a)) * pow(r, 8.0) + point;
	//	//
	//	//#endif        
	//	//			Vector3 wAbs = w.absolute();
	//	//
	//	//			trapW = Vector3(min(trapW.x, wAbs.x), min(trapW.y, wAbs.y), min(trapW.z, wAbs.z));
	//	//			trapM = min(trapM, m);
	//	//
	//	//			m = Vector3::dotProduct(w, w);
	//	//
	//	//			if (m > 256.0)
	//	//				break;
	//	//		}
	//	//
	//	//		outputColour = Vector3(m, trapW.y, trapW.z);
	//	//
	//	//		// distance estimation (through the Hubbard-Douady potential)
	//	//		return 0.25 * log(m) * sqrt(m) / dz;


	//}



	//float opUnion(float d1, float d2)
	//{
	//	return min(d1, d2);
	//}

	//float opSubtraction(float d1, float d2)
	//{
	//	return max(-d1, d2);
	//}

	//float opIntersection(float d1, float d2)
	//{
	//	return max(d1, d2);
	//}

	//// polynomial smooth min
	//float smin(float a, float b, float k)
	//{
	//	float h = max(k - abs(a - b), 0.0) / k;
	//	return min(a, b) - h * h * k * (1.0 / 4.0);
	//}

	//float opSmoothUnion(float d1, float d2, float k)
	//{
	//	float h = max(k - abs(d1 - d2), 0.0);
	//	return min(d1, d2) - h * h * 0.25 / k;
	//	//float h = clamp( 0.5 + 0.5*(d2-d1)/k, 0.0, 1.0 );
	//	//return mix( d2, d1, h ) - k*h*(1.0-h);
	//}

	//float opSmoothSubtraction(float d1, float d2, float k)
	//{
	//	float h = max(k - abs(-d1 - d2), 0.0);
	//	return max(-d1, d2) + h * h * 0.25 / k;
	//	//float h = clamp( 0.5 - 0.5*(d2+d1)/k, 0.0, 1.0 );
	//	//return mix( d2, -d1, h ) + k*h*(1.0-h);
	//}

	//float opSmoothIntersection(float d1, float d2, float k)
	//{
	//	float h = max(k - abs(d1 - d2), 0.0);
	//	return max(d1, d2) + h * h * 0.25 / k;
	//	//float h = clamp( 0.5 - 0.5*(d2-d1)/k, 0.0, 1.0 );
	//	//return mix( d2, d1, h ) + k*h*(1.0-h);
	//}

	//Vector3 phong(const Vector3& n, const Vector3& v)
	//{
	//	//material parameters
	//	const float ks = 3.0;
	//	const float kd = 3.0;
	//	const float ka = 1.0;
	//	const float al = 20.0;
	//	//light parameters
	//	const float ia = 1.0;
	//	const Vector3 lm = Vector3(5, 3, -1).normalised();
	//	const float id = 1.0;
	//	const float is = 1.0;



	//	Vector3 rm = n * 2.0 * Vector3::dotProduct(lm, n) - lm;

	//	float ip = ka * ia + (kd * clamp01(Vector3::dotProduct(lm, n)) * id + ks * pow(clamp01(Vector3::dotProduct(rm, v)), al) * is);

	//	return Vector3(0.1, 0.2, 0.5) * ip;
	//}


};