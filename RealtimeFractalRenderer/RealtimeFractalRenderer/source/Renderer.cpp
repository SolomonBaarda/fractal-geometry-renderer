#include "Renderer.h"
#include "constants.h"

#include <iostream>


Renderer::Renderer() : Renderer(900, 600) { }

Renderer::Renderer(uint32_t width, uint32_t height) : width(width), height(height), size(width * height)
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
	int err;

	// Get platform ID
	if (clGetPlatformIDs(1, &platform, NULL) != CL_SUCCESS)
	{
		printf("Error: Failed to get platform\n");
		return EXIT_FAILURE;
	}

	// Connect to a compute device
	if (clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL) != CL_SUCCESS)
	{
		printf("Error: Failed to create a device group!\n");
		return EXIT_FAILURE;
	}

	// Create a compute context 
	context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
	if (!context)
	{
		printf("Error: Failed to create a compute context!\n");
		return EXIT_FAILURE;
	}

	// Create a command commands
	commands = clCreateCommandQueue(context, device_id, 0, &err);
	if (!commands)
	{
		printf("Error: Failed to create a command commands!\n");
		return EXIT_FAILURE;
	}

	return 0;
}

int Renderer::load_kernel(std::string path)
{
	FILE* file = fopen(path.c_str(), "rb");

	if (file == NULL)
	{
		printf("Error: Failed to load from file\n");
		exit(1);
	}

	// Convert to string buffer in memory
	fseek(file, 0, SEEK_END);
	size_t program_size = ftell(file);
	rewind(file);
	char* program_buffer = (char*)malloc(program_size + 1);
	program_buffer[program_size] = '\0';
	fread(program_buffer, sizeof(char), program_size, file);
	fclose(file);

	int err;

	// Create the compute program from the source buffer
	program = clCreateProgramWithSource(context, 1, (const char**)&program_buffer, &program_size, &err);
	if (!program)
	{
		printf("Error: Failed to create compute program!\n");
		exit(1);
	}

	// Build the program executable
	err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if (err != CL_SUCCESS)
	{
		size_t len;
		char buffer[2048];

		printf("Error: Failed to build program executable!\n");
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
		printf("%s\n", buffer);
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

void Renderer::render(float time)
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


	cl_float3 pos;
	pos.x = -10;
	pos.y = -sin(time * 0.5f) * 5.0f;
	pos.z = -10;

	cl_float3 look;
	look.x = 0;
	look.y = 0;
	look.z = 0;

	cl_float fov = 40.0f;
	cl_float aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
	cl_float focus = 0.1f;

	err |= clSetKernelArg(kernel, 3, sizeof(cl_float3), &pos);
	err |= clSetKernelArg(kernel, 4, sizeof(cl_float3), &look);
	err |= clSetKernelArg(kernel, 5, sizeof(float), &fov);
	err |= clSetKernelArg(kernel, 6, sizeof(float), &aspect_ratio);
	err |= clSetKernelArg(kernel, 7, sizeof(float), &focus);


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

	// Set local for now
	local = 200;

	// Execute the kernel over the entire range of our 1d input data set
	// using the maximum number of work group items for this device
	global = size;
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

