#pragma once

#include "Renderer.h"
#include "Window.h"
#include "Benchmark.h"
#include "Timer.h"

#include <cstdint>
#include <cstdio>
#include <string>
#include <chrono>

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
		Window w;
		Renderer r;

	public:
		FractalGeometryRenderer(uint32_t width, uint32_t height) : w(width, height), r(width, height)
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
		void run(std::string scene_kernel_path, std::string build_options, size_t specified_work_group_size)
		{
			Scene scene = r.load_scene(scene_kernel_path, build_options, specified_work_group_size);

			Profiling::Timer timer;
			Events events;
			Camera camera(scene.camera_up_axis);
			camera.position = scene.get_camera_position_at_time(0);
			camera.facing = scene.get_camera_facing_direction_at_time(0);

			// Flush any events that occured before now
			w.get_events();

			bool running = true;
			float total_time_seconds = 0;

			Profiling::Benchmark benchmark("Total frame time");

			printf("\n");


			do
			{
				printf("Delta time: %f\n", timer.getLastDeltaTimeSeconds());

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
				events = w.get_events();
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
					printf("Camera position: (%.1f, %.1f, %.1f) facing: (%.1f, %.1f, %.1f)\n", camera.position.x(), camera.position.y(), camera.position.z(), camera.facing.x(), camera.facing.y(), camera.facing.z());
				}

				benchmark.addMarkerNow("update camera");

				// Render the scene
				r.render(camera, total_time_seconds);
				benchmark.addMarkerNow("render to buffer");

				w.set_pixels(r.buffer);
				benchmark.addMarkerNow("render buffer to window");

				if (events.take_screenshot)
				{
					//timer.stop();

					// Calculate filename
					const auto p1 = std::chrono::system_clock::now();
					int64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(p1.time_since_epoch()).count();
					// Save screenshot
					r.save_screenshot(std::to_string(ms) + ".ppm");

					//timer.resume();
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
		}
	};
}


// Documentation for the main Doxygen page

/// @mainpage
///
/// 
/// @section section_installation_guide Installation Guide
/// text
/// 
/// @section section_user_guide User Guide
/// text
/// 