#define CL_TARGET_OPENCL_VERSION 220

#include "Renderer.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

Renderer::Renderer() : Renderer(900, 600) { }

Renderer::Renderer(uint32_t width, uint32_t height) : width(width), height(height), size(width* height), platforms(), devices(),
b("Render to buffer"), aspect_ratio(static_cast<float>(width) / static_cast<float>(height))
{
	// Create buffer objects for new resolution
	resolution_changed();
	// Setup OpenCL objects
	setup();

	b.start();
}

Renderer::~Renderer()
{
	delete screen_coordinates;
	delete buffer;
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

			screen_coordinates[index].x = static_cast<float>(x) / static_cast<float>(width - 1);
			screen_coordinates[index].y = static_cast<float>(y) / static_cast<float>(height - 1);
		}
	}
}

static std::vector<std::string> split(std::string s, std::string delimiter)
{
	uint32_t start = 0;
	size_t end = s.find(delimiter);
	std::vector<std::string> strings;

	while (end != std::string::npos)
	{
		strings.push_back(s.substr(start, end - start));
		start = end + delimiter.length();
		end = s.find(delimiter, start);
	}

	return strings;
}

void Renderer::setup()
{
	platform_id = 0;
	device_id = 0;

	// Get platforms and devices
	cl::Platform::get(&platforms);
	platforms.at(platform_id).getDevices(CL_DEVICE_TYPE_GPU, &devices);

	std::vector<cl::Device> devices_temp;

	// Debug resolution information
	printf("Running at resolution: %ux%u\n", width, height);
	printf("\n");


	// Debug all available platforms and devices
	printf("Available platforms and devices:\n");
	for (int32_t i = 0; i < platforms.size(); i++)
	{
		printf("\t%s (%s)\n", platforms.at(i).getInfo<CL_PLATFORM_NAME>().c_str(), platforms.at(i).getInfo<CL_PLATFORM_VERSION>().c_str());

		platforms.at(platform_id).getDevices(CL_DEVICE_TYPE_GPU, &devices_temp);
		for (int32_t j = 0; j < devices_temp.size(); j++)
		{
			printf("\t\t%s (%s)\n", devices_temp.at(j).getInfo<CL_DEVICE_NAME>().c_str(), devices_temp.at(j).getInfo<CL_DEVICE_VERSION>().c_str());
		}
	}
	printf("\n");


	// Debug info about the chosen platform and device
	printf("Chosen platform: %s (%s)\n", platforms.at(platform_id).getInfo<CL_PLATFORM_NAME>().c_str(), platforms.at(platform_id).getInfo<CL_PLATFORM_VERSION>().c_str());
	printf("Chosen device: %s (%s)\n", devices.at(device_id).getInfo<CL_DEVICE_NAME>().c_str(), devices.at(device_id).getInfo<CL_DEVICE_VERSION>().c_str());

	printf("\tMax clock frequency: %u MHz\n", devices.at(device_id).getInfo<CL_DEVICE_MAX_CLOCK_FREQUENCY>());
	printf("\tNumber of parallel compute units: %u\n", devices.at(device_id).getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>());

	printf("\tMax number of work items for each dimension of the work group: ");
	for (size_t size : devices.at(device_id).getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>())
	{
		printf("%zu ", size); // %zu code for size_t
	}
	printf("\n");

	printf("\tOpenCL C version: %s\n", devices.at(device_id).getInfo<CL_DEVICE_OPENCL_C_VERSION>().c_str());
	printf("\n");


	// Debug supported extentions for this device
	printf("Device supports extentions:\n");
	std::vector<std::string> extentions = split(devices.at(device_id).getInfo<CL_DEVICE_EXTENSIONS>(), " ");
	for (std::string s : extentions)
	{
		printf("\t%s\n", s.c_str());
	}
	printf("\n");

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



Scene Renderer::load_scene_details()
{
	cl_int error_code = 0;

	// Now create a kernel to load scene information 
	cl::Kernel load_scene_kernel = cl::Kernel(program, "getSceneInformation", &error_code);

	if (error_code != CL_SUCCESS)
	{
		printf("Error: Failed to create scene loading kernel %d\n", error_code);
		exit(1);
	}

	cl::Buffer camera_up_axis_buffer = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_float3), NULL, &error_code);

	const cl_uint camera_arrays_capacity = 8u;

	cl::Buffer camera_positions_size_buffer = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_uint), NULL, &error_code);
	cl::Buffer camera_positions_at_time_buffer = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_float4) * camera_arrays_capacity, NULL, &error_code);

	cl::Buffer camera_facing_size_buffer = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_uint), NULL, &error_code);
	cl::Buffer camera_facing_at_time_buffer = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_float4) * camera_arrays_capacity, NULL, &error_code);

	cl::Buffer camera_do_loop_buffer = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(bool), NULL, &error_code);

	if (error_code != CL_SUCCESS)
	{
		printf("Error: Failed to allocate device memory %d\n", error_code);
		exit(1);
	}

	error_code |= load_scene_kernel.setArg(0, sizeof(cl_mem), &camera_up_axis_buffer);

	error_code |= load_scene_kernel.setArg(1, sizeof(cl_uint), &camera_arrays_capacity);

	error_code |= load_scene_kernel.setArg(2, sizeof(cl_mem), &camera_positions_size_buffer);
	error_code |= load_scene_kernel.setArg(3, sizeof(cl_mem), &camera_positions_at_time_buffer);

	error_code |= load_scene_kernel.setArg(4, sizeof(cl_mem), &camera_facing_size_buffer);
	error_code |= load_scene_kernel.setArg(5, sizeof(cl_mem), &camera_facing_at_time_buffer);

	error_code |= load_scene_kernel.setArg(6, sizeof(cl_mem), &camera_do_loop_buffer);

	if (error_code != CL_SUCCESS)
	{
		printf("Error: Failed to set kernel arguments load scene %d\n", error_code);
		exit(1);
	}

	// Now set the work group size in the kernel
	error_code = commands.enqueueNDRangeKernel(load_scene_kernel, 1, 1, 1);
	if (error_code != CL_SUCCESS)
	{
		printf("Error: Failed to execute kernel %d\n", error_code);
		exit(1);
	}

	// Wait for the commands to execute
	commands.finish();






	cl_float4 camera_positions_at_time[camera_arrays_capacity];
	cl_float4 camera_facing_directions_at_time[camera_arrays_capacity];
	cl_float3 camera_up_axis;
	cl_uint positions_size, facing_size;
	cl_bool do_camera_loop;




	// Read the output from the buffer
	error_code |= commands.enqueueReadBuffer(camera_up_axis_buffer, CL_TRUE, 0, sizeof(cl_float3), &camera_up_axis);

	error_code |= commands.enqueueReadBuffer(camera_positions_size_buffer, CL_TRUE, 0, sizeof(cl_uint), &positions_size);
	error_code |= commands.enqueueReadBuffer(camera_positions_at_time_buffer, CL_TRUE, 0, sizeof(cl_float4) * camera_arrays_capacity, &camera_positions_at_time);

	error_code |= commands.enqueueReadBuffer(camera_facing_size_buffer, CL_TRUE, 0, sizeof(cl_uint), &facing_size);
	error_code |= commands.enqueueReadBuffer(camera_facing_at_time_buffer, CL_TRUE, 0, sizeof(cl_float4) * camera_arrays_capacity, &camera_facing_directions_at_time);

	error_code |= commands.enqueueReadBuffer(camera_do_loop_buffer, CL_TRUE, 0, sizeof(bool), &do_camera_loop);

	if (error_code != CL_SUCCESS)
	{
		printf("Error: Failed to read output data %d\n", error_code);
		exit(1);
	}

	commands.finish();



	std::vector <std::pair<Vector3, float>> vec_camera_positions_at_time;
	std::vector <std::pair<Vector3, float>> vec_camera_facing_directions_at_time;



	// Add positions at time
	for (int32_t i = 0; i < camera_arrays_capacity && i < positions_size; i++)
	{
		Vector3 position(camera_positions_at_time[i].x, camera_positions_at_time[i].y, camera_positions_at_time[i].z);
		float time = camera_positions_at_time[i].w;

		vec_camera_positions_at_time.push_back(std::pair(position, time));
	}

	// Add facing at time
	for (int32_t i = 0; i < camera_arrays_capacity && i < facing_size; i++)
	{
		Vector3 facing(camera_facing_directions_at_time[i].x, camera_facing_directions_at_time[i].y, camera_facing_directions_at_time[i].z);
		facing.normalise();
		float time = camera_facing_directions_at_time[i].w;

		vec_camera_facing_directions_at_time.push_back(std::pair(facing, time));
	}

	Scene s(Vector3(camera_up_axis.x, camera_up_axis.y, camera_up_axis.z), vec_camera_positions_at_time, vec_camera_facing_directions_at_time, do_camera_loop);

	return s;
}



Scene Renderer::load_scene(std::string scene_kernel_path, std::string build_options)
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


	Scene s = load_scene_details();

	// Create the compute kernel
	kernel = cl::Kernel(program, "calculatePixelColour", &error_code);

	if (error_code != CL_SUCCESS)
	{
		printf("Error: Failed to create main compute kernel %d\n", error_code);
		exit(1);
	}


	// Get the work group sizes
	size_t max_work_group_size = kernel.getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(devices.at(device_id));
	size_t preferred_work_group_size = kernel.getWorkGroupInfo<CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE>(devices.at(device_id), &error_code);

	number_work_items = size;
	work_group_size = preferred_work_group_size;

	// Debug info about the work group sizes
	printf("Number of work items (number of pixels): %zu\n", number_work_items);
	printf("Maximum work group size for kernel: %zu\n", max_work_group_size);
	printf("Preferred work group size for kernel: %zu\n", preferred_work_group_size);
	printf("Chosen work group size: %zu\n", work_group_size);
	printf("\n");


	// Create input and output buffers
	screen_coordinate_input = cl::Buffer(context, CL_MEM_READ_ONLY, sizeof(cl_float2) * size, NULL, &error_code);
	colours_output = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(uint8_t) * size * 4, NULL, &error_code);

	if (error_code != CL_SUCCESS)
	{
		printf("Error: Failed to allocate device memory %d\n", error_code);
		exit(1);
	}



	// Write screen coordinates for each pixel into the buffer
	// We only need to do this once
	error_code = commands.enqueueWriteBuffer(screen_coordinate_input, CL_TRUE, 0, sizeof(cl_float2) * size, screen_coordinates);

	if (error_code != CL_SUCCESS)
	{
		printf("Error: Failed to write to source array %d\n", error_code);
		exit(1);
	}

	error_code = kernel.setArg(0, sizeof(cl_mem), &screen_coordinate_input);

	if (error_code != CL_SUCCESS)
	{
		printf("Error: Failed to set kernel screen coordinate arguments %d\n", error_code);
		exit(1);
	}

	return s;
}

void Renderer::render(const Camera& camera, float time)
{
	b.addMarkerNow("start of render");

	cl_float3 pos;
	pos.x = camera.position.x;
	pos.y = camera.position.y;
	pos.z = camera.position.z;

	cl_float3 facing;
	facing.x = camera.facing.x;
	facing.y = camera.facing.y;
	facing.z = camera.facing.z;

	// Set the kernel arguments
	cl_int error_code = 0;
	error_code |= kernel.setArg(1, sizeof(cl_mem), &colours_output);
	error_code |= kernel.setArg(2, sizeof(uint32_t), &size);
	error_code |= kernel.setArg(3, sizeof(float), &time);
	error_code |= kernel.setArg(4, sizeof(cl_float3), &pos);
	error_code |= kernel.setArg(5, sizeof(cl_float3), &facing);
	error_code |= kernel.setArg(6, sizeof(float), &aspect_ratio);

	if (error_code != CL_SUCCESS)
	{
		printf("Error: Failed to set kernel arguments %d\n", error_code);
		exit(1);
	}

	b.addMarkerNow("set arguments");

	// Now set the work group size in the kernel
	error_code = commands.enqueueNDRangeKernel(kernel, 1, number_work_items, work_group_size);
	if (error_code != CL_SUCCESS)
	{
		printf("Error: Failed to execute kernel %d\n", error_code);
		exit(1);
	}

	// Wait for the commands to execute
	commands.finish();

	b.addMarkerNow("wait for completion");


	// Read the output from the buffer
	error_code = commands.enqueueReadBuffer(colours_output, CL_TRUE, 0, sizeof(uint8_t) * size * 4, buffer);

	if (error_code != CL_SUCCESS)
	{
		printf("Error: Failed to read output array %d\n", error_code);
		exit(1);
	}

	commands.finish();

	b.addMarkerNow("read buffer");
}

void Renderer::save_screenshot(std::string path)
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
