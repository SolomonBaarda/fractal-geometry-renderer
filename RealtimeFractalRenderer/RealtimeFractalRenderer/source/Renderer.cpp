//#define CL_HPP_TARGET_OPENCL_VERSION 220

//#define CL_HPP_USE_IL_KHR






#include "Renderer.h"

#include <fstream>
#include <iostream>


Renderer::Renderer() : Renderer(900, 600) { }

Renderer::Renderer(uint32_t width, uint32_t height) : width(width), height(height), size(width* height)
{
	screen_coordinates = new cl_float2[size];

	//#pragma omp parallel for schedule(dynamic, 1)  // OpenMP

	// Fill buffer with screen coordinate data
	for (int32_t y = 0; y < height; y++)
	{
		for (int32_t x = 0; x < width; x++)
		{
			float u = static_cast<float>(x) / (width - 1);
			float v = static_cast<float>(y) / (height - 1);

			uint32_t index = y * width + x;
			screen_coordinates[index].x = u;
			screen_coordinates[index].y = v;
		}
	}

	buffer = new uint8_t[size * 4];

	setup();
}

Renderer::~Renderer()
{
	cleanup();
}

int Renderer::setup()
{
	cl_int err;

	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);
	printf("Running on platform: %s (%s)\n", platforms[0].getInfo<CL_PLATFORM_NAME>().c_str(), platforms[0].getInfo<CL_PLATFORM_VERSION>().c_str());

	std::vector<cl::Device> devices;
	platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &devices);

	printf("Running on device: %s (%s)\n", devices[0].getInfo<CL_DEVICE_NAME>().c_str(), devices[0].getInfo<CL_DEVICE_VERSION>().c_str());
	//printf("CL_DEVICE_ADDRESS_BITS is %d for this device.\n", devices[0].getInfo<CL_DEVICE_ADDRESS_BITS>());
	//printf("Device supports extentions: %s\n", devices[0].getInfo<CL_DEVICE_EXTENSIONS>().c_str());

	char* versions = new char[1024];
	size_t actual = 0;

	err = clGetDeviceInfo(devices[0](), CL_DEVICE_IL_VERSION, 1024 * sizeof(char), versions, &actual);
	printf("IL version: %s\n", versions);
	//printf("Device running on version: %s\n", devices[0].getInfo<CL_DEVICE_IL_VERSION>().c_str());


	

	// Get platform ID
	if (clGetPlatformIDs(1, &platform_id, NULL) != CL_SUCCESS)
	{
		printf("Error: Failed to get platform\n");
		return EXIT_FAILURE;
	}

	// Connect to a compute device
	if (clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL) != CL_SUCCESS)
	{
		printf("Error: Failed to create a device group!\n");
		return EXIT_FAILURE;
	}

	// Create a compute context 
	context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
	if (err != CL_SUCCESS)
	{
		printf("Error: Failed to create a compute context!\n");
		return EXIT_FAILURE;
	}

	// Create a command commands
	commands = clCreateCommandQueue(context, device_id, 0, &err);
	if (err != CL_SUCCESS)
	{
		printf("Error: Failed to create a command commands!\n");
		return EXIT_FAILURE;
	}

	return 0;
}

static std::vector<cl_uchar> readSPIRVFromFile(const std::string& filename)
{
	std::ifstream is(filename, std::ios::binary);
	std::vector<cl_uchar> ret;

	if (!is.good())
	{
		printf("Couldn't open file '%s'\n", filename.c_str());
		exit(1);
		return ret;
	}

	size_t filesize = 0;
	is.seekg(0, std::ios::end);
	filesize = (size_t)is.tellg();
	is.seekg(0, std::ios::beg);

	ret.reserve(filesize);
	ret.insert(ret.begin(), std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>());

	return ret;
}

int Renderer::load_kernel(std::string path)
{
#ifdef CL_VERSION_2_2

	cl::Device d(device_id);
	std::string version = d.getInfo<CL_DEVICE_VERSION>();

	cl_int err = 0;
	std::vector<cl_uchar> spirv = readSPIRVFromFile(path);

	program = clCreateProgramWithIL(context, (const void*)spirv.data(), spirv.size(), &err);

	if (err != CL_SUCCESS)
	{
		printf("Error: Failed to create program with IL (code: %d)\n", err);
		exit(1);
	}

	err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if (err != CL_SUCCESS)
	{
		// Determine the reason for the error
		char buildLog[2048];
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buildLog), buildLog, NULL);

		std::cerr << "Error in program: " << std::endl;
		std::cerr << buildLog << std::endl;
		clReleaseProgram(program);
		exit(1);
	}

	// Create the compute kernel in the program we wish to run
	kernel = clCreateKernel(program, "calculatePixelColour", &err);
	if (!kernel || err != CL_SUCCESS)
	{
		printf("Error: Failed to create compute kernel!\n");
		exit(1);
	}

	// Create the input and output arrays in device memory for our calculation
	screen_coordinate_input = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(cl_float2) * size, NULL, NULL);
	colours_output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(uint8_t) * size * 4, NULL, NULL);
	if (!screen_coordinate_input || !colours_output)
	{
		printf("Error: Failed to allocate device memory!\n");
		exit(1);
	}

#elif

	printf("Error: Must use OpenCL 2.2 or newer\n");
	exit(1);

#endif

	return 0;
}

void Renderer::cleanup()
{
	clReleaseMemObject(screen_coordinate_input);
	clReleaseMemObject(colours_output);
	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(commands);
	clReleaseContext(context);
}

void Renderer::render(const Camera& camera, float time)
{
	// Write our data set into the input array in device memory 
	int err = 0;
	err |= clEnqueueWriteBuffer(commands, screen_coordinate_input, CL_TRUE, 0, sizeof(cl_float2) * size, screen_coordinates, 0, NULL, NULL);
	if (err != CL_SUCCESS)
	{
		printf("Error: Failed to write to source array!\n");
		exit(1);
	}

	// Set the arguments to our compute kernel
	err = 0;
	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &screen_coordinate_input);
	err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &colours_output);
	err |= clSetKernelArg(kernel, 2, sizeof(uint32_t), &size);
	err |= clSetKernelArg(kernel, 3, sizeof(float), &time);

	cl_float3 pos;
	pos.x = camera.position.x;
	pos.y = camera.position.y;
	pos.z = camera.position.z;

	cl_float3 facing;
	facing.x = camera.facing.x;
	facing.y = camera.facing.y;
	facing.z = camera.facing.z;

	cl_float aspect_ratio = static_cast<float>(width) / static_cast<float>(height);

	err |= clSetKernelArg(kernel, 4, sizeof(cl_float3), &pos);
	err |= clSetKernelArg(kernel, 5, sizeof(cl_float3), &facing);
	err |= clSetKernelArg(kernel, 6, sizeof(float), &camera.vertical_fov);
	err |= clSetKernelArg(kernel, 7, sizeof(float), &aspect_ratio);
	err |= clSetKernelArg(kernel, 8, sizeof(float), &camera.foucs_distance);


	if (err != CL_SUCCESS)
	{
		printf("Error: Failed to set kernel arguments! %d\n", err);
		exit(1);
	}

	// Get the maximum work group size for executing the kernel on the device
	err = clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
	if (err != CL_SUCCESS)
	{
		printf("Error: Failed to retrieve kernel work group info! %d\n", err);
		exit(1);
	}

	global = size;
	local = calculate_local_work_group_size(global);

	err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global, &local, 0, NULL, NULL);
	if (err != CL_SUCCESS)
	{
		printf("Error: Failed to execute kernel!\n");
		exit(1);
	}

	// Wait for the command commands to get serviced before reading back results
	clFinish(commands);

	// Read back the results from the device to verify the output
	err = clEnqueueReadBuffer(commands, colours_output, CL_TRUE, 0, sizeof(uint8_t) * size * 4, buffer, 0, NULL, NULL);
	if (err != CL_SUCCESS)
	{
		printf("Error: Failed to read output array! %d\n", err);
		exit(1);
	}

}

size_t Renderer::calculate_local_work_group_size(size_t global_size)
{
	size_t max;

	if (clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(max), &max, NULL) != CL_SUCCESS)
	{
		printf("Error: Failed to get maximum work group size");
		exit(1);
	}

	return max;
}


