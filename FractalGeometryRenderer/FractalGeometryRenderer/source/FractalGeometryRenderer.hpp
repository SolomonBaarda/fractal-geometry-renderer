#pragma once

#include "Renderer.h"
#include "Benchmark.h"
#include "Timer.h"

#ifndef NO_GUI_BUILD
#include "Window.h"
#endif

#include <cstdint>
#include <cstdio>
#include <string>
#include <chrono>
#include <ostream>

/// <summary>
/// The main namespace containing the FractalGeometryRenderer class and its respective components.
/// </summary>
namespace FractalGeometryRenderer
{
	/// <summary>
	/// The main class driving the FractalGeometryRenderer.
	/// </summary>
	class FractalGeometryRenderer
	{
	private:

#ifndef NO_GUI_BUILD
		Window w;
#endif
		Renderer r;
		uint32_t width, height;
		std::ostream& log;

	public:
		FractalGeometryRenderer(uint32_t width, uint32_t height, std::ostream& log) : width(width), height(height),

#ifndef NO_GUI_BUILD
			w(width, height, log),
#endif
			r(width, height, log), log(log)
		{ }

		/// <summary>
		/// Method to start the application. 
		/// </summary>
		/// <param name="scene_kernel_path">A path relative to the current directory for the the scene .cl kernel file</param>
		/// <param name="build_options">Options to be passed to the OpenCL compiler. Check the OpenCL C++ wrapper documentation 
		/// for a full list of options. If any additional include directories are required, then the build options should be 
		/// edited to reflect this. If this is done, the kernels/include directory must also be added to the list of include 
		/// directories </param>
		/// <param name="specified_work_group_size">Desired group size when distrubuting work over the GPU</param>
		void run(std::string scene_kernel_path, std::string build_options, size_t specified_work_group_size, std::ostream& data)
		{
			DeviceStats stats = r.getDeviceData();

			Scene scene = r.load_scene(scene_kernel_path, build_options, specified_work_group_size);

			Profiling::Timer timer;
			Events events;
			Camera camera(scene.camera_up_axis, 30.0f, scene.camera_speed);
			camera.position = scene.get_camera_position_at_time(0);
			camera.facing = scene.get_camera_facing_direction_at_time(0);

#ifndef NO_GUI_BUILD
			// Flush any events that occured before now
			w.get_events();
#endif

			bool running = true;
			double total_time_seconds = 0;

			Profiling::Benchmark benchmark("Total frame time", log);

			log << "\n";

			do
			{
				timer.start();

				// Check if the benchmark needs to be started
				// Do this if we aren't doing a time specific start/stop benchmark, or if we are past the start time
				if (!benchmark.getIsRunning() && (!scene.do_timed_benchmark ||
					(scene.do_timed_benchmark && scene.benchmark_start_stop_time.first <= total_time_seconds)))
				{
					benchmark.start();
				}

				benchmark.addMarkerNow("start of frame");

				// Process events
#ifndef NO_GUI_BUILD
				events = w.get_events();
#endif
				running = !events.exit;

				benchmark.addMarkerNow("poll events");

				// Update objects in the scene
				if (scene.allow_user_camera_control)
				{
					// Use keyboard input to update the camera position
					camera.update(events, timer.getLastDeltaTimeSeconds());
				}
				else
				{
					// Set the values manually by lerping between values depending on the time
					camera.position = scene.get_camera_position_at_time(total_time_seconds);
					camera.facing = scene.get_camera_facing_direction_at_time(total_time_seconds);
				}

				if (events.debug_information)
				{
					printf("Camera position: (%.5f, %.5f, %.5f) facing: (%.2f, %.2f, %.2f)\n", camera.position.x(), camera.position.y(), camera.position.z(), camera.facing.x(), camera.facing.y(), camera.facing.z());
				}

				benchmark.addMarkerNow("update camera");

				// Render the scene
				r.render(camera, total_time_seconds);
				benchmark.addMarkerNow("render to buffer");

#ifndef NO_GUI_BUILD
				w.set_pixels(r.buffer);
#endif
				benchmark.addMarkerNow("render buffer to window");

				if (events.take_screenshot)
				{
					timer.pause();

					// Calculate filename
					const auto p1 = std::chrono::system_clock::now();
					int64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(p1.time_since_epoch()).count();
					// Save screenshot
					r.save_screenshot(std::to_string(ms) + ".ppm");

					timer.resume();
				}
				benchmark.addMarkerNow("take screenshot");

				// Check if the benchmark needs to be stopped
				// Do this if we are past the start finish time for the benchmark
				if (benchmark.getIsRunning() && scene.do_timed_benchmark && scene.benchmark_start_stop_time.second <= total_time_seconds)
				{
					running = false;
				}

				// Must be the last lines of the main loop
				timer.stop();
				total_time_seconds += timer.getLastDeltaTimeSeconds();
				benchmark.recordFrameTime(timer.getLastDeltaTimeSeconds());


			} while (running);

			benchmark.stop();

			// Output data to file
			const std::string delim(",");

			data <<
				scene_kernel_path << delim <<
				build_options << delim <<

				width << delim <<
				height << delim <<

				stats.name << delim <<
				stats.version << delim <<
				r.getWorkGroupSize() << delim <<
				stats.clock_freq_mhz << delim <<
				stats.parallel_compute_units << delim <<
				stats.global_memory_size_bytes << delim <<
				stats.local_memory_size_bytes << delim <<
				stats.constant_memory_size_bytes << delim <<

				benchmark.total_frame_time_seconds << delim <<
				benchmark.total_number_frames << delim <<
				benchmark.maximum_frame_time_seconds << delim <<
				benchmark.minimum_frame_time_seconds << "\n";
		}
	};

	// Documentation for the main Doxygen pages

	/// @mainpage Home
	/// 
	/// @section section_system_requirements System Requirements
	/// Your device must contain a GPU that contains the OpenCL 1.2 runtime or newer. If your device contains multiple GPUs
	/// then the first one found by the program will be chosen.
	/// 
	/// @section section_installation_guide Installation Guide
	/// Pre-built binaries can be downloaded from the GitHub releases page https://github.com/SolomonBaarda/fractal-geometry-renderer/releases.
	/// 
	/// @section section_user_guide User Guide
	/// The application should be run from the command line. For example, using cmd on Windows the Mandelbulb scene could
	/// be run using 
	/// > FractalGeometryRenderer -s "kernels/mandelbulb.cl" -r 1920 1080
	/// 
	/// <table>
	/// <caption id="multi_row">Command Line Options</caption>
	/// <tr>	<th>Command						<th>Parameters						<th>Description
	/// <tr>	<td>-h,--help					<td>								<td>Lists available commands
	/// <tr>	<td><b>-s,--scene</b>			<td>String/path						<td>Path to the scene to load
	/// <tr>	<td>-i,--include				<td>List of strings/paths			<td>Additional file paths to be included by the OpenCL compiler
	/// <tr>	<td>-r,--resolution				<td>Width and height as integers	<td>Resolution of the window to create
	/// <tr>	<td>-w,--work-group-size		<td>Positive integer				<td>Manually specify the size of each work group (max is usually 255 for most devices)
	/// <tr>	<td>-f,--force-high-precision	<td>								<td>Manually disable the fast-maths OpenCL compiler optimisations. This will result in a significant performance loss
	/// </table>
	/// 
	/// By default, the kernels/include directory is included by the application. If your scene uses includes outside 
	/// of this file, then the path must be specified when running the application using -i
	/// 
	/// Documentation for @ref page_scene_development
	/// 
	/// Documentation for @ref page_custom_builds
	/// 
	

	/// @page page_custom_builds Manual Builds
	///
	/// The project can be cloned from the github repository https://github.com/SolomonBaarda/fractal-geometry-renderer, and built manually using CMake.
	/// The following packages must be installed on your system:
	/// 
	/// <table>
	/// <caption id="multi_row">Packages</caption>
	/// <tr>	<th>Package		<th>Version
	/// <tr>	<td>OpenCL		<td>2.2
	/// <tr>	<td>SDL2		<td>2.0
	/// <tr>	<td>Eigen3		<td>3.3
	/// </table>
	/// 
	/// In addition, to build and run the unit tests GTest must be installed.
	/// 
	/// Documentation for the FractalGeometryRenderer.



}
