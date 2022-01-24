#pragma once

#include "Camera.h"
#include "Benchmark.h"
#include "Scene.h"

// Use the C++ bindings for fancy object wrappers
#include <CL/cl.hpp>

#include <cstdint>
#include <string>
#include <vector>

namespace FractalGeometryRenderer
{
	/// <summary>
	/// The renderer of the application, responsible for calculating the colour for each pixel in the window.
	/// </summary>
	class Renderer
	{
	public:
		Renderer();
		Renderer(uint32_t width, uint32_t height);
		~Renderer();

		/// <summary>
		/// Renders the camera's view of the current scene to the colour buffer.
		/// </summary>
		/// <param name="camera">A camera viewing the current scene</param>
		/// <param name="time">The current time since creation for the scene</param>
		void render(const Scene& scene, const Camera& camera, float time);

		/// <summary>
		/// A buffer containing the most recent RGBA 0-255 colours. Array has capacity width x height. 
		/// </summary>
		uint8_t* buffer;


		/// <summary>
		/// Attempts to load the scene from the specified kernel .cl path. 
		/// </summary>
		/// <param name="scene_kernel_path">A path relative to the current directory for the the scene .cl kernel file</param>
		/// <param name="build_options">Options to be passed to the OpenCL compiler. Check the OpenCL C++ wrapper 
		/// documentation for a full list of options</param>
		/// <returns>A scene object for the specified kernel</returns>
		Scene load_scene(std::string scene_kernel_path, std::string build_options);

		/// <summary>
		/// Saves the contents of the colour buffer to a ppm format image, at the specified path.
		/// </summary>
		/// <param name="path">A valid path for the image to be created at. It must use already existing directories.</param>
		void save_screenshot(std::string path);

	private:
		const uint32_t width, height, size;
		const cl_float aspect_ratio;

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


		Profiling::Benchmark b;

		Scene load_scene_details();



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
}