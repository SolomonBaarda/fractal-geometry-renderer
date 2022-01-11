#define CL_TARGET_OPENCL_VERSION 220

#include "Renderer.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

Renderer::Renderer() : Renderer(900, 600) { }

Renderer::Renderer(uint32_t width, uint32_t height) : width(width), height(height), size(width* height), platforms(), devices()
{
	// Create buffer objects for new resolution
	resolution_changed();
	// Setup OpenCL objects
	setup();
}

void Renderer::resolution_changed()
{
	// Create input and output buffers
	screen_coordinates = new cl_float2[size];
	buffer = new uint8_t[static_cast<int64_t>(size) * static_cast<int64_t>(4)];

	// Fill buffer with screen coordinate data
	// Use OpenMP to speed this up
#pragma omp parallel for schedule(dynamic, 1)  
	for (int32_t y = 0; y < height; y++)
	{
		for (int32_t x = 0; x < width; x++)
		{
			int32_t index = y * width + x;

			screen_coordinates[index].x = static_cast<float>(x) / (width - 1);
			screen_coordinates[index].y = static_cast<float>(y) / (height - 1);
		}
	}
}

void Renderer::setup()
{
	platform_id = 0;
	device_id = 0;

	// Get all platforms
	cl::Platform::get(&platforms);
	printf("Running on platform: %s (%s)\n", platforms.at(platform_id).getInfo<CL_PLATFORM_NAME>().c_str(), platforms.at(platform_id).getInfo<CL_PLATFORM_VERSION>().c_str());

	// Get all devices
	platforms.at(platform_id).getDevices(CL_DEVICE_TYPE_GPU, &devices);
	printf("Running on device: %s (%s)\n", devices.at(device_id).getInfo<CL_DEVICE_NAME>().c_str(), devices.at(device_id).getInfo<CL_DEVICE_VERSION>().c_str());
	//printf("CL_DEVICE_ADDRESS_BITS is %d for this device.\n", devices[0].getInfo<CL_DEVICE_ADDRESS_BITS>());
	//printf("Device supports extentions: %s\n", devices[0].getInfo<CL_DEVICE_EXTENSIONS>().c_str());

	// Create context
	context = cl::Context(devices.at(device_id));

	// Create command queue
	commands = cl::CommandQueue(context, devices.at(device_id));
}

static std::string readTextFromFile(const std::string& filename)
{
	std::ifstream is(filename, std::ios::in);
	std::stringstream buffer;

	if (!is.good())
	{
		printf("Couldn't open file '%s'\n", filename.c_str());
		exit(1);
	}

	buffer << is.rdbuf();

	return buffer.str();
}

void Renderer::load_kernel(std::string scene_kernel_path, std::string build_options)
{
	cl_int error_code = 0;

	// Load the source code from the kernel
	cl::STRING_CLASS scene_kernel_source(readTextFromFile(scene_kernel_path));

	// Create and build the program
	program = cl::Program(context, scene_kernel_source, &error_code);

	if (error_code != CL_SUCCESS)
	{
		printf("Error: Failed to create main program %d\n", error_code);
		exit(1);
	}
	
	error_code = program.build(build_options.c_str());

	if (error_code != CL_SUCCESS)
	{
		// If the build fails, print the build logs for all devices
		for (auto& device : program.getInfo<CL_PROGRAM_DEVICES>())
		{
			printf("Program build log for device %s:\n", device.getInfo<CL_DEVICE_NAME>().c_str());
			printf("%s\n", program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device).c_str());
		}
		exit(1);
	}

	// Create the compute kernel
	kernel = cl::Kernel(program, "calculatePixelColour", &error_code);

	if (error_code != CL_SUCCESS)
	{
		printf("Error: Failed to create compute kernel %d\n", error_code);
		exit(1);
	}

	// Create input and output buffers
	screen_coordinate_input = cl::Buffer(context, CL_MEM_READ_ONLY, sizeof(cl_float2) * size, NULL, &error_code);
	colours_output = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(uint8_t) * size * 4, NULL, &error_code);

	if (error_code != CL_SUCCESS)
	{
		printf("Error: Failed to allocate device memory %d\n", error_code);
		exit(1);
	}
}

void Renderer::render(const Camera& camera, float time)
{
	// Write screen coordinates for each pixel into the buffer
	cl_int error_code = commands.enqueueWriteBuffer(screen_coordinate_input, CL_TRUE, 0, sizeof(cl_float2) * size, screen_coordinates);

	if (error_code != CL_SUCCESS)
	{
		printf("Error: Failed to write to source array %d\n", error_code);
		exit(1);
	}

	cl_float3 pos;
	pos.x = camera.position.x;
	pos.y = camera.position.y;
	pos.z = camera.position.z;

	cl_float3 facing;
	facing.x = camera.facing.x;
	facing.y = camera.facing.y;
	facing.z = camera.facing.z;

	cl_float aspect_ratio = static_cast<float>(width) / static_cast<float>(height);

	// Set the kernel arguments
	error_code |= kernel.setArg(0, sizeof(cl_mem), &screen_coordinate_input);
	error_code |= kernel.setArg(1, sizeof(cl_mem), &colours_output);
	error_code |= kernel.setArg(2, sizeof(uint32_t), &size);
	error_code |= kernel.setArg(3, sizeof(float), &time);
	error_code |= kernel.setArg(4, sizeof(cl_float3), &pos);
	error_code |= kernel.setArg(5, sizeof(cl_float3), &facing);
	error_code |= kernel.setArg(6, sizeof(float), &camera.vertical_fov);
	error_code |= kernel.setArg(7, sizeof(float), &aspect_ratio);
	error_code |= kernel.setArg(8, sizeof(float), &camera.foucs_distance);

	if (error_code != CL_SUCCESS)
	{
		printf("Error: Failed to set kernel arguments %d\n", error_code);
		exit(1);
	}

	// Get the local work group size
	global = size;
	local = calculate_local_work_group_size(global);

	// Now set the work group size in the kernel
	error_code = commands.enqueueNDRangeKernel(kernel, 1, global, local);
	if (error_code != CL_SUCCESS)
	{
		printf("Error: Failed to execute kernel %d\n", error_code);
		exit(1);
	}

	// Wait for the commands to execute
	commands.finish();

	// Read the output from the buffer
	error_code = commands.enqueueReadBuffer(colours_output, CL_TRUE, 0, sizeof(uint8_t) * size * 4, buffer);

	if (error_code != CL_SUCCESS)
	{
		printf("Error: Failed to read output array %d\n", error_code);
		exit(1);
	}

}

size_t Renderer::calculate_local_work_group_size(size_t global_size)
{
	cl_int error_code = 0;
	size_t local = kernel.getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(devices.at(device_id), &error_code);

	if (error_code != CL_SUCCESS)
	{
		printf("Error: Failed to retrieve kernel work group info %d\n", error_code);
		exit(1);
	}

	return local;
}
