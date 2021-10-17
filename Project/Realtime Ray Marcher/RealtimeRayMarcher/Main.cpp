#define PROGRAM_FILE ".\\kernels\\trace.cl"
#define KERNEL_FUNC "getColourForPixel"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <iostream>
#include "Vector3.h"

#ifdef MAC
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif


/* Find a GPU or CPU associated with the first available platform
The `platform` structure identifies the first platform identified by the
OpenCL runtime. A platform identifies a vendor's installation, so a system
may have an NVIDIA platform and an AMD platform.
The `device` structure corresponds to the first accessible device
associated with the platform. Because the second parameter is
`CL_DEVICE_TYPE_GPU`, this device must be a GPU.
*/
cl_device_id create_device() {

	cl_platform_id platform;
	cl_device_id dev;
	int err;

	/* Identify a platform */
	err = clGetPlatformIDs(1, &platform, NULL);
	if (err < 0) {
		perror("Couldn't identify a platform");
		exit(1);
	}

	// Access a device
	// GPU
	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &dev, NULL);
	if (err == CL_DEVICE_NOT_FOUND) {
		// CPU
		err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &dev, NULL);
	}
	if (err < 0) {
		perror("Couldn't access any devices");
		exit(1);
	}

	return dev;
}





/* Create program from a file and compile it */
cl_program build_program(cl_context ctx, cl_device_id dev, const char* filename) {
	cl_program program;
	FILE* program_handle;
	char* program_buffer, * program_log;
	size_t program_size, log_size;
	int err;

	/* Read program file and place content into buffer */
	program_handle = fopen(filename, "r");
	if (program_handle == NULL) {
		perror("Couldn't find the program file");
		exit(1);
	}
	fseek(program_handle, 0, SEEK_END);
	program_size = ftell(program_handle);
	rewind(program_handle);
	program_buffer = (char*)malloc(program_size + 1);
	program_buffer[program_size] = '\0';
	fread(program_buffer, sizeof(char), program_size, program_handle);
	fclose(program_handle);

	/* Create program from file
	Creates a program from the source code in the add_numbers.cl file.
	Specifically, the code reads the file's content into a char array
	called program_buffer, and then calls clCreateProgramWithSource.
	*/
	program = clCreateProgramWithSource(ctx, 1,
		(const char**)&program_buffer, &program_size, &err);
	if (err < 0) {
		perror("Couldn't create the program");
		exit(1);
	}
	free(program_buffer);

	/* Build program
	The fourth parameter accepts options that configure the compilation.
	These are similar to the flags used by gcc. For example, you can
	define a macro with the option -DMACRO=VALUE and turn off optimization
	with -cl-opt-disable.
	*/
	err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if (err < 0) {

		/* Find size of log and print to std output */
		clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG,
			0, NULL, &log_size);
		program_log = (char*)malloc(log_size + 1);
		program_log[log_size] = '\0';
		clGetProgramBuildInfo(program, dev, CL_PROGRAM_BUILD_LOG,
			log_size + 1, program_log, NULL);
		printf("%s\n", program_log);
		free(program_log);
		exit(1);
	}

	return program;
}



















static float max(float a, float b)
{
	return a > b ? a : b;
}

static float min(float a, float b)
{
	return a < b ? a : b;
}


inline float clamp(float x)
{
	return x < 0 ? 0 : x > 1 ? 1 : x;
}

inline int32_t toInt(float x)
{
	// Applies a gamma correction of 2.2
	return static_cast<int32_t>(pow(clamp(x), 1 / 2.2) * 255 + .5);
}

void saveImageToFile(float image[], const int32_t width, const int32_t height, const char* filename)
{
	FILE* f = fopen(filename, "w"); // Write image to PPM file.
	fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);

	// Rows
	for (int32_t y = 0; y < height; y++)
	{		// Columns
		for (int32_t x = 0; x < width; x++)
		{
			int32_t index = (y * width + x) * 3;
			fprintf(f, "%d %d %d ", toInt(image[index]), toInt(image[index + 1]), toInt(image[index + 2]));
		}

	}

	fclose(f);
}








int main() {

	/* OpenCL structures */
	cl_device_id device;
	cl_context context;
	cl_program program;
	cl_kernel kernel;
	cl_command_queue queue;
	cl_int err;
	cl_int num_groups;






	const float aspect_ratio = 16.0f / 9.0f;

	// Image
	const int32_t width = 1024, height = width / 1.777f;
	const int32_t total_pixels = width * height, array_size = total_pixels * 3;


	float colours [array_size];
	float rayPositions[array_size];
	float rayDirections [array_size];


	// Camera
	const float viewport_height = 2.0f;
	const float viewport_width = aspect_ratio * viewport_height;
	const float focal_length = 1.0f;

	const Vector3 origin(0, 0, 0);
	const Vector3 horizontal(viewport_width, 0, 0);
	const Vector3 vertical(0, viewport_height, 0);
	const Vector3 lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vector3(0, 0, focal_length);



	// ********************* set the data

	// Rows
	for (int32_t y = 0; y < height; y++)
	{
		fprintf(stderr, "\rRendering %5.2f%%", 100.0f * static_cast<float>(y) / static_cast<float>(height - 1));

		// Columns
		for (int32_t x = 0; x < width; x++)
		{
			float u = static_cast<float>(x) / (width - 1);
			float v = static_cast<float>(y) / (height - 1);

			Vector3 screenPosition = lower_left_corner + horizontal * u + vertical * v - origin;
			Vector3 rayPosition = origin + screenPosition;
			Vector3 direction = screenPosition.normalised();

			int32_t index = (y * width + x) * 3;

			rayPositions[index] = screenPosition.x;
			rayPositions[index + 1] = screenPosition.y;
			rayPositions[index + 2] = screenPosition.z;

			rayDirections[index] = direction.x;
			rayDirections[index + 1] = direction.y;
			rayDirections[index + 2] = direction.z;
		}
	}





	/* Create device and context
	Creates a context containing only one device — the device structure
	created earlier.
	*/
	device = create_device();
	context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
	if (err < 0) {
		perror("Couldn't create a context");
		exit(1);
	}

	/* Build program */
	program = build_program(context, device, PROGRAM_FILE);

	/* Create data buffer
	• `global_size`: total number of work items that will be
	   executed on the GPU (e.g. total size of your array)
	• `local_size`: size of local workgroup. Each workgroup contains
	   several work items and goes to a compute unit
	In this example, the kernel is executed by eight work-items divided into
	two work-groups of four work-items each. Returning to my analogy,
	this corresponds to a school containing eight students divided into
	two classrooms of four students each.
	  Notes:
	• Intel recommends workgroup size of 64-128. Often 128 is minimum to
	get good performance on GPU
	• On NVIDIA Fermi, workgroup size must be at least 192 for full
	utilization of cores
	• Optimal workgroup size differs across applications
	*/
	size_t global_size = 8; // WHY ONLY 8?
	size_t local_size = 4;
	num_groups = global_size / local_size;


	// *********************** create buffers

	const float memory_size = sizeof(float) * array_size;

	cl_mem outputColours = clCreateBuffer(context, CL_MEM_WRITE_ONLY, memory_size, NULL, &err);

	if (err < 0) {
		perror("Couldn't create a buffer 0");
		exit(1);
	};

	cl_mem inputPositions = clCreateBuffer(context, CL_MEM_READ_ONLY || CL_MEM_USE_HOST_PTR, memory_size, rayPositions, &err);

	if (err < 0) {
		perror("Couldn't create a buffer 1");
		exit(1);
	};
	cl_mem inputDirections = clCreateBuffer(context, CL_MEM_READ_ONLY || CL_MEM_USE_HOST_PTR, memory_size, rayDirections, &err);

	if (err < 0) {
		perror("Couldn't create a buffer 2");
		exit(1);
	};




	//err = clEnqueueWriteBuffer(inputPositions, inputPositions, CL_TRUE, 0, memory_size, rayPositions, 0, NULL, NULL);

	//if (err < 0) {
	//	perror("Couldn't write data to buffers");
	//	exit(1);
	//}

	/* Create a command queue
	Does not support profiling or out-of-order-execution
	*/
	queue = clCreateCommandQueueWithProperties(context, device, 0, &err);
	if (err < 0) {
		perror("Couldn't create a command queue");
		exit(1);
	};

	/* Create a kernel */
	kernel = clCreateKernel(program, KERNEL_FUNC, &err);
	if (err < 0) {
		perror("Couldn't create a kernel");
		exit(1);
	};

	/* Create kernel arguments */
	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &inputPositions); // <=====INPUT
	err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &inputDirections);
	err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &outputColours); // <=====OUTPUT
	if (err < 0) {
		perror("Couldn't create a kernel argument");
		exit(1);
	}

	/* Enqueue kernel
	At this point, the application has created all the data structures
	(device, kernel, program, command queue, and context) needed by an
	OpenCL host application. Now, it deploys the kernel to a device.
	Of the OpenCL functions that run on the host, clEnqueueNDRangeKernel
	is probably the most important to understand. Not only does it deploy
	kernels to devices, it also identifies how many work-items should
	be generated to execute the kernel (global_size) and the number of
	work-items in each work-group (local_size).
	*/
	err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_size,
		&local_size, 0, NULL, NULL);
	if (err < 0) {
		perror("Couldn't enqueue the kernel");
		exit(1);
	}

	/* Read the kernel's output    */
	err = clEnqueueReadBuffer(queue, outputColours, CL_TRUE, 0, sizeof(colours), colours, 0, NULL, NULL); // <=====GET OUTPUT
	if (err < 0) {
		perror("Couldn't read the buffer");
		exit(1);
	}




	saveImageToFile(colours, width, height, "image.ppm");








	/* Deallocate resources */
	clReleaseKernel(kernel);
	clReleaseMemObject(inputDirections);
	clReleaseMemObject(inputPositions);
	clReleaseMemObject(outputColours);
	clReleaseCommandQueue(queue);
	clReleaseProgram(program);
	clReleaseContext(context);
	return 0;
}


