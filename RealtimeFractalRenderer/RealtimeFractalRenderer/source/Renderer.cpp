#include "Renderer.h"
#include "constants.h"

#include <iostream>


Renderer::Renderer() : Renderer(900, 600) { }

Renderer::Renderer(uint32_t width, uint32_t height) : width(width), height(height)
{
	float aspect_ratio = static_cast<float>(width) / static_cast<float>(height);

	camera = Camera(Vector3(-10, -5, -10), Vector3(0, 0, 0), Vector3(0, 1, 0), 40.0f, aspect_ratio, 0.1f);
	buffer = new Vector3[static_cast<int64_t>(width) * static_cast<int64_t>(height)];


    setup();
}

Renderer::~Renderer()
{
    cleanup();
}

int Renderer::setup()
{
    int err;

    // Fill our data set with random float values
    int i = 0;
    unsigned int count = DATA_SIZE;
    for (i = 0; i < count; i++)
        data[i] = rand() / (float)RAND_MAX;


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
    char * program_buffer = (char*)malloc(program_size + 1);
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
    kernel = clCreateKernel(program, "square", &err);
    if (!kernel || err != CL_SUCCESS)
    {
        printf("Error: Failed to create compute kernel!\n");
        exit(1);
    }

    return 0;
}

int Renderer::run()
{
    int err;
    unsigned int count = DATA_SIZE;

    // Create the input and output arrays in device memory for our calculation
    input = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * count, NULL, NULL);
    output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * count, NULL, NULL);
    if (!input || !output)
    {
        printf("Error: Failed to allocate device memory!\n");
        exit(1);
    }

    // Write our data set into the input array in device memory 
    err = clEnqueueWriteBuffer(commands, input, CL_TRUE, 0, sizeof(float) * count, data, 0, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to write to source array!\n");
        exit(1);
    }

    // Set the arguments to our compute kernel
    err = 0;
    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &output);
    err |= clSetKernelArg(kernel, 2, sizeof(unsigned int), &count);
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

    // Execute the kernel over the entire range of our 1d input data set
    // using the maximum number of work group items for this device
    global = count;
    err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global, &local, 0, NULL, NULL);
    if (err)
    {
        printf("Error: Failed to execute kernel!\n");
        return EXIT_FAILURE;
    }

    // Wait for the command commands to get serviced before reading back results
    clFinish(commands);

    // Read back the results from the device to verify the output
    err = clEnqueueReadBuffer(commands, output, CL_TRUE, 0, sizeof(float) * count, results, 0, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to read output array! %d\n", err);
        exit(1);
    }

    // Validate our results
    correct = 0;
    for (int i = 0; i < count; i++)
    {
        if (results[i] == data[i] * data[i])
            correct++;
    }

    // Print a brief summary detailing the results
    printf("Computed '%d/%d' correct values!\n", correct, count);
}

void Renderer::cleanup()
{
    clReleaseMemObject(input);
    clReleaseMemObject(output);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(commands);
    clReleaseContext(context);
}

void Renderer::render()
{
#pragma omp parallel for schedule(dynamic, 1)  // OpenMP

	// Rows
	for (int32_t y = 0; y < height; y++)
	{
		//fprintf(stderr, "\rRendering %5.2f%%", 100.0f * static_cast<float>(y) / static_cast<float>(height - 1));

		// Columns
		for (int32_t x = 0; x < width; x++)
		{
			float u = static_cast<float>(x) / (width - 1);
			float v = static_cast<float>(y) / (height - 1);

			Ray r = camera.getCameraRay(u, v);
			buffer[y * width + x] = calculatePixelColour(r.origin, r.direction);
		}
	}
}

