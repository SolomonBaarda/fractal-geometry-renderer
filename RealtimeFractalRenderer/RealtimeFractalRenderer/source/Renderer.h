#pragma once

#include "Camera.h"
#include "Benchmark.h"

// Use the C++ bindings for fancy object wrappers
#include <CL/cl.hpp>

#include <cstdint>
#include <string>
#include <vector>

class Renderer
{
public:
	Renderer();
	Renderer(uint32_t width, uint32_t height);

	void render(const Camera &camera, float time);

	uint8_t* buffer;

	void load_kernel(std::string scene_kernel_path, std::string build_options);

	void save_screenshot(std::string path)
	{
		FILE* f = fopen(path.c_str(), "w"); // Write image to PPM file.
		fprintf(f, "P3\n%d %d\n%d\n", static_cast<int32_t>(width), static_cast<int32_t>(height), 255);

		for (int32_t i = 0; i < width * height; i++)
		{
			int32_t index = i * 4;
			fprintf(f, "%d %d %d ", buffer[index], buffer[index + 1], buffer[index + 2]);
		}

		fclose(f);

		printf("Saved screenshot as %s\n", path.c_str());
	}

private:
	const uint32_t width, height, size;
	const float aspect_ratio;

	std::vector<cl::Platform> platforms;
	std::vector<cl::Device> devices;
	uint32_t platform_id, device_id;

	cl::Context context;
	cl::CommandQueue commands;

	cl::Program program;
	cl::Kernel kernel;
	size_t number_work_items;
	size_t work_group_size;

	cl_float2* screen_coordinates;
	cl::Buffer screen_coordinate_input;

	cl::Buffer colours_output;

	void setup();
	void resolution_changed();


	Benchmark b;





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