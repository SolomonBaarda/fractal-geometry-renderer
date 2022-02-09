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
		Renderer(uint32_t width, uint32_t height);
		~Renderer();

		/// <summary>
		/// Renders the camera's view of the current scene to the colour buffer.
		/// </summary>
		/// <param name="camera">A camera viewing the current scene</param>
		/// <param name="time">The current time since creation for the scene</param>
		void render(const Camera& camera, double time);

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
		/// <param name="work_group_size">Desired group size when distrubuting work over the GPU</param>
		/// <returns>A scene object for the specified kernel</returns>
		Scene load_scene(std::string scene_kernel_path, std::string build_options, size_t work_group_size);

		/// <summary>
		/// Saves the contents of the colour buffer to a ppm format image, at the specified path.
		/// </summary>
		/// <param name="path">A valid path for the image to be created at. It must use already existing directories.</param>
		void save_screenshot(std::string path);

	private:
		const cl_uint width, height, size;

		std::vector<cl::Platform> platforms;
		std::vector<cl::Device> devices;
		uint32_t platform_id, device_id;

		cl::Context context;
		cl::CommandQueue commands;

		cl::Program program;
		cl::Kernel kernel;
		size_t number_work_items;
		size_t work_group_size;

		cl::Buffer colours_output;

		void setup();


		Profiling::Benchmark b;

		Scene load_scene_details();


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