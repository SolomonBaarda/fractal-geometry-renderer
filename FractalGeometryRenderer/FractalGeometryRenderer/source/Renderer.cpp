#define CL_TARGET_OPENCL_VERSION 220

#include "Renderer.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

namespace FractalGeometryRenderer
{
	Renderer::Renderer(uint32_t width, uint32_t height, std::ostream& log) : width(width), height(height), size(width* height), log(log), platforms(), devices(), b("Render to buffer", log)
	{
		// Create buffer objects for new resolution
		buffer = new uint8_t[static_cast<int64_t>(size) * static_cast<int64_t>(4)];

		// Setup OpenCL objects
		setup();

		b.start();
	}

	Renderer::~Renderer()
	{
		delete buffer;
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

		// Debug all available platforms and devices
		log << "Available platforms and devices:\n";
		for (int32_t i = 0; i < platforms.size(); i++)
		{
			log << "\t" << platforms.at(i).getInfo<CL_PLATFORM_NAME>() << " (" << platforms.at(i).getInfo<CL_PLATFORM_VERSION>() << ")\n";

			platforms.at(platform_id).getDevices(CL_DEVICE_TYPE_GPU, &devices_temp);
			for (int32_t j = 0; j < devices_temp.size(); j++)
			{
				log << "\t\t" << devices_temp.at(j).getInfo<CL_DEVICE_NAME>() << " (" << devices_temp.at(j).getInfo<CL_DEVICE_VERSION>() << ")\n";
			}
		}
		log << "\n";


		// Debug info about the chosen platform and device
		log << "Chosen platform: " << platforms.at(platform_id).getInfo<CL_PLATFORM_NAME>() << " (" << platforms.at(platform_id).getInfo<CL_PLATFORM_VERSION>() << ")\n";
		log << "Chosen device: " << devices.at(device_id).getInfo<CL_DEVICE_NAME>() << " (" << devices.at(device_id).getInfo<CL_DEVICE_VERSION>() << ")\n";

		log << "\tMax clock frequency: " << devices.at(device_id).getInfo<CL_DEVICE_MAX_CLOCK_FREQUENCY>() << " MHz\n";
		log << "\tNumber of parallel compute units: " << devices.at(device_id).getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>() << "\n";
		log << "\tGlobal memory size: " << devices.at(device_id).getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>() / 1e+9 << " GB\n";
		log << "\tLocal memory size: " << devices.at(device_id).getInfo<CL_DEVICE_LOCAL_MEM_SIZE>() / 1000.0f << " KB\n";
		log << "\tConstant memory size: " << devices.at(device_id).getInfo<CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE>() / 1000.0f << " KB\n";


		log << "\tMax work items for each dimension of the work group: ";
		for (size_t size : devices.at(device_id).getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>())
		{
			log << size << " ";
		}
		log << "\n";

		log << "\tOpenCL C version: " << devices.at(device_id).getInfo<CL_DEVICE_OPENCL_C_VERSION>() << "\n";
		log << "\n";


		// Debug supported extentions for this device
		log << "Device supports extentions:\n";
		std::vector<std::string> extentions = split(devices.at(device_id).getInfo<CL_DEVICE_EXTENSIONS>(), " ");
		for (std::string s : extentions)
		{
			log << "\t" << s << "\n";
		}
		log << "\n";

		// Create context
		context = cl::Context(devices.at(device_id));

		// Create command queue
		commands = cl::CommandQueue(context, devices.at(device_id));
	}

	DeviceStats Renderer::getDeviceData()
	{
		DeviceStats stats;

		stats.name = devices.at(device_id).getInfo<CL_DEVICE_NAME>();
		stats.version = devices.at(device_id).getInfo<CL_DEVICE_VERSION>();
		stats.clock_freq_mhz = devices.at(device_id).getInfo<CL_DEVICE_MAX_CLOCK_FREQUENCY>();
		stats.parallel_compute_units = devices.at(device_id).getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>();
		stats.global_memory_size_bytes = devices.at(device_id).getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>();
		stats.local_memory_size_bytes = devices.at(device_id).getInfo<CL_DEVICE_LOCAL_MEM_SIZE>();
		stats.constant_memory_size_bytes = devices.at(device_id).getInfo<CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE>();

		return stats;
	}

	std::string Renderer::readTextFromFile(const std::string& filename)
	{
		std::ifstream is(filename, std::ios::in);
		std::stringstream buffer;

		if (!is.good())
		{
			log << "Couldn't open file " << filename.c_str() << "\n";
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
			log << "Error: Failed to create scene loading kernel " << error_code << "\n";
			exit(1);
		}

		const cl_uint array_capacity = 8u;
		cl::Buffer camera_up_axis_buffer = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_float3), NULL, &error_code);
		cl::Buffer camera_positions_size_buffer = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_uint), NULL, &error_code);
		cl::Buffer camera_positions_at_time_buffer = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_float4) * array_capacity, NULL, &error_code);
		cl::Buffer camera_facing_size_buffer = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_uint), NULL, &error_code);
		cl::Buffer camera_facing_at_time_buffer = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_float4) * array_capacity, NULL, &error_code);
		cl::Buffer camera_do_loop_buffer = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_bool), NULL, &error_code);
		cl::Buffer camera_speed_buffer = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_float), NULL, &error_code);
		cl::Buffer benchmark_start_stop_time_buffer = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_float2), NULL, &error_code);

		if (error_code != CL_SUCCESS)
		{
			log << "Error: Failed to allocate device memory " << error_code << "\n";
			exit(1);
		}

		error_code |= load_scene_kernel.setArg(0, sizeof(cl_mem), &camera_up_axis_buffer);
		error_code |= load_scene_kernel.setArg(1, sizeof(cl_uint), &array_capacity);
		error_code |= load_scene_kernel.setArg(2, sizeof(cl_mem), &camera_positions_size_buffer);
		error_code |= load_scene_kernel.setArg(3, sizeof(cl_mem), &camera_positions_at_time_buffer);
		error_code |= load_scene_kernel.setArg(4, sizeof(cl_mem), &camera_facing_size_buffer);
		error_code |= load_scene_kernel.setArg(5, sizeof(cl_mem), &camera_facing_at_time_buffer);
		error_code |= load_scene_kernel.setArg(6, sizeof(cl_mem), &camera_do_loop_buffer);
		error_code |= load_scene_kernel.setArg(7, sizeof(cl_mem), &camera_speed_buffer);
		error_code |= load_scene_kernel.setArg(8, sizeof(cl_mem), &benchmark_start_stop_time_buffer);

		if (error_code != CL_SUCCESS)
		{
			log << "Error: Failed to set kernel arguments load scene " << error_code << "\n";
			exit(1);
		}

		// Now set the work group size in the kernel
		error_code = commands.enqueueNDRangeKernel(load_scene_kernel, 1, 1, 1);
		if (error_code != CL_SUCCESS)
		{
			log << "Error: Failed to execute kernel " << error_code << "\n";
			exit(1);
		}

		// Wait for the commands to execute
		commands.finish();

		cl_float4 camera_positions_at_time[array_capacity];
		cl_float4 camera_rotations_at_time[array_capacity];
		cl_float3 camera_up_axis;
		cl_uint positions_size, facing_size;
		cl_bool do_camera_loop;
		cl_float camera_speed;
		cl_float2 benchmark_start_stop_time;

		// Read the output from the buffer
		error_code |= commands.enqueueReadBuffer(camera_up_axis_buffer, CL_TRUE, 0, sizeof(cl_float3), &camera_up_axis);
		error_code |= commands.enqueueReadBuffer(camera_positions_size_buffer, CL_TRUE, 0, sizeof(cl_uint), &positions_size);
		error_code |= commands.enqueueReadBuffer(camera_positions_at_time_buffer, CL_TRUE, 0, sizeof(cl_float4) * array_capacity, &camera_positions_at_time);
		error_code |= commands.enqueueReadBuffer(camera_facing_size_buffer, CL_TRUE, 0, sizeof(cl_uint), &facing_size);
		error_code |= commands.enqueueReadBuffer(camera_facing_at_time_buffer, CL_TRUE, 0, sizeof(cl_float4) * array_capacity, &camera_rotations_at_time);
		error_code |= commands.enqueueReadBuffer(camera_do_loop_buffer, CL_TRUE, 0, sizeof(cl_bool), &do_camera_loop);
		error_code |= commands.enqueueReadBuffer(camera_speed_buffer, CL_TRUE, 0, sizeof(cl_float), &camera_speed);
		error_code |= commands.enqueueReadBuffer(benchmark_start_stop_time_buffer, CL_TRUE, 0, sizeof(cl_float2), &benchmark_start_stop_time);

		if (error_code != CL_SUCCESS)
		{
			log << "Error: Failed to read output data " << error_code << "\n";
			exit(1);
		}

		commands.finish();

		std::vector <std::pair<Eigen::Vector3f, float>> vec_camera_positions_at_time;
		std::vector <std::pair<Eigen::Vector3f, float>> vec_camera_facing_directions_at_time;

		if (positions_size > array_capacity)
		{
			log << "Warning: too many camera positions have been specified (max=" << array_capacity << ")\n";
		}
		if (facing_size > array_capacity)
		{
			log << "Warning: too many camera facing directions have been specified (max=" << array_capacity << ")\n";
		}

		// Add positions at time
		for (int32_t i = 0; i < array_capacity && i < positions_size; i++)
		{
			Eigen::Vector3f position(camera_positions_at_time[i].x, camera_positions_at_time[i].y, camera_positions_at_time[i].z);
			float time = camera_positions_at_time[i].w;

			vec_camera_positions_at_time.push_back(std::pair(position, time));
		}

		// Add facing at time
		for (int32_t i = 0; i < array_capacity && i < facing_size; i++)
		{
			Eigen::Vector3f facing(camera_rotations_at_time[i].x, camera_rotations_at_time[i].y, camera_rotations_at_time[i].z);
			facing.normalize();
			float time = camera_rotations_at_time[i].w;

			vec_camera_facing_directions_at_time.push_back(std::pair(facing, time));
		}

		Scene s(Eigen::Vector3f(camera_up_axis.x, camera_up_axis.y, camera_up_axis.z), vec_camera_positions_at_time,
			vec_camera_facing_directions_at_time, do_camera_loop, camera_speed, std::pair(benchmark_start_stop_time.x, benchmark_start_stop_time.y));

		return s;
	}

	size_t Renderer::calculateWorkGroupSize(size_t total_work_items, size_t maximum_work_group_size, size_t desired_work_group_size)
	{
		if (total_work_items % 2 != 0)
		{
			log << "Error: Work items (total number of pixels " << total_work_items << ") must be a multiple of 2\n";
			exit(1);
		}

		// User has specified a work group size
		if (desired_work_group_size > 0)
		{
			if (desired_work_group_size > maximum_work_group_size)
			{
				log << "Error: Desired work group size (" << desired_work_group_size << ") must be smaller or equal to the maximum work group size (" << total_work_items << ")\n";
				exit(1);
			}

			if (total_work_items % desired_work_group_size != 0)
			{
				log << "Error: Desired work group size (" << desired_work_group_size << ") must be a multiple of the total work items (total number of pixels " << total_work_items << ")\n";
				exit(1);
			}

			return desired_work_group_size;
		}
		// Use default work group size
		else
		{
			return maximum_work_group_size;
		}
	}

	Scene Renderer::load_scene(std::string scene_kernel_path, std::string build_options, size_t user_specified_work_group_size)
	{
		cl_int error_code = 0;

		// Load the source code from the kernel
		cl::STRING_CLASS scene_kernel_source(readTextFromFile(scene_kernel_path));

		// Create and build the program
		program = cl::Program(context, scene_kernel_source, &error_code);

		if (error_code != CL_SUCCESS)
		{
			log << "Error: Failed to create main program " << error_code << "\n";
			exit(1);
		}

		error_code = program.build(build_options.c_str());


		if (error_code != CL_SUCCESS)
		{
			// If the build fails, print the build logs for all devices
			for (auto& device : program.getInfo<CL_PROGRAM_DEVICES>())
			{
				log << "Program build log for device " << device.getInfo<CL_DEVICE_NAME>() << ":\n";
				log << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << "\n";
			}
			exit(1);
		}

		Scene s = load_scene_details();

		// Create the compute kernel
		kernel = cl::Kernel(program, "calculatePixelColour", &error_code);

		if (error_code != CL_SUCCESS)
		{
			log << "Error: Failed to create main compute kernel " << error_code << "\n";
			exit(1);
		}


		// Get the work group sizes
		size_t max_work_group_size = kernel.getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(devices.at(device_id));
		size_t preferred_work_group_size = kernel.getWorkGroupInfo<CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE>(devices.at(device_id), &error_code);

		number_work_items = size;

		// Debug info about the work group sizes
		log << "\n";
		log << "Number of work items (number of pixels): " << number_work_items << "\n";
		log << "Maximum work group size for this kernel: " << max_work_group_size << "\n";
		log << "Compiler hint work group size for this kernel: " << preferred_work_group_size << "\n";

		work_group_size = calculateWorkGroupSize(number_work_items, max_work_group_size, user_specified_work_group_size);

		log << "Chosen work group size: " << work_group_size << "\n";
		log << "\n";


		// Create input and output buffers
		colours_output = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(uint8_t) * size * 4, NULL, &error_code);

		if (error_code != CL_SUCCESS)
		{
			log << "Error: Failed to allocate device memory " << error_code << "\n";
			exit(1);
		}

		// Set the kernel arguments that won't change
		error_code |= kernel.setArg(0, sizeof(cl_mem), &colours_output);
		error_code |= kernel.setArg(1, sizeof(cl_uint), &width);
		error_code |= kernel.setArg(2, sizeof(cl_uint), &height);

		if (error_code != CL_SUCCESS)
		{
			log << "Error: Failed to set constant kernel arguments " << error_code << "\n";
			exit(1);
		}

		return s;
	}

	void Renderer::render(const Camera& camera, double time)
	{
		b.addMarkerNow("start of render");

		cl_float time_cl = time;

		cl_float3 pos;
		pos.x = camera.position.x();
		pos.y = camera.position.y();
		pos.z = camera.position.z();

		cl_float3 facing;
		facing.x = camera.facing.x();
		facing.y = camera.facing.y();
		facing.z = camera.facing.z();

		// Set the kernel arguments
		cl_int error_code = 0;
		error_code |= kernel.setArg(3, sizeof(cl_float), &time_cl);
		error_code |= kernel.setArg(4, sizeof(cl_float3), &pos);
		error_code |= kernel.setArg(5, sizeof(cl_float3), &facing);

		if (error_code != CL_SUCCESS)
		{
			log << "Error: Failed to set kernel arguments " << error_code << "\n";
			exit(1);
		}

		b.addMarkerNow("set arguments");

		// Now set the work group size in the kernel
		error_code = commands.enqueueNDRangeKernel(kernel, 1, number_work_items, work_group_size);
		if (error_code != CL_SUCCESS)
		{
			log << "Error: Failed to execute kernel " << error_code << "\n";
			exit(1);
		}

		// Wait for the commands to execute
		commands.finish();

		b.addMarkerNow("wait for completion");


		// Read the output from the buffer
		error_code = commands.enqueueReadBuffer(colours_output, CL_TRUE, 0, sizeof(uint8_t) * size * 4, buffer);

		if (error_code != CL_SUCCESS)
		{
			log << "Error: Failed to read output array " << error_code << "\n";
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

		log << "Saved screenshot as " << path << "\n";
	}
}