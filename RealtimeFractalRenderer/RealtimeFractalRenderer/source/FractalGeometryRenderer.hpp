#pragma once

#include "Renderer.h"
#include "Window.h"
#include "Benchmark.h"

#include "Timer.h"

#include <cstdint>
#include <cstdio>
#include <string>
#include <chrono>

class FractalGeometryRenderer
{
private:
	Window w;
	Renderer r;

public:
	FractalGeometryRenderer() : FractalGeometryRenderer(1920, 1080)
	{ }

	FractalGeometryRenderer(uint32_t width, uint32_t height) : w(width, height), r(width, height)
	{ }

	void run(std::string scene_path, std::string build_options = "-I kernels/include")
	{
		Scene scene = r.load_scene(scene_path, build_options);

		Timer timer;
		Events events;
		Camera camera(scene.camera_up_axis);
		camera.position = scene.camera_positions_at_time.at(0).first;
		camera.facing = scene.camera_facing_directions_at_time.at(0).first;
		//Camera::calculatePitchAndYaw(camera.facing, &camera.pitch, &camera.yaw);

		// Flush any events that occured before now
		w.get_events();

		bool running = true;
		float total_time_seconds = 0;

		Benchmark benchmark("Total frame time");


		printf("\n");


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
			events = w.get_events();
			running = !events.exit;

			benchmark.addMarkerNow("poll events");

			// Update objects in the scene
			if (scene.allow_user_camera_control)
			{
				// Use keyboard input to update the camera position
				camera.update(events, timer.delta_time_seconds);
			}
			else
			{
				// Set the values manually by lerping between values depending on the time
				camera.position = scene.get_camera_value_at_time(scene.camera_positions_at_time, total_time_seconds, scene.do_camera_loop);
				camera.facing = scene.get_camera_value_at_time(scene.camera_facing_directions_at_time, total_time_seconds, scene.do_camera_loop);
			}

			if (events.debug_information)
			{
				printf("Camera position: (%.1f, %.1f, %.1f) facing: (%.1f, %.1f, %.1f)\n", camera.position.x, camera.position.y, camera.position.z, camera.facing.x, camera.facing.y, camera.facing.z);
			}

			benchmark.addMarkerNow("update camera");

			// Render the scene
			r.render(camera, total_time_seconds);
			benchmark.addMarkerNow("render to buffer");

			w.set_pixels(r.buffer);
			benchmark.addMarkerNow("render buffer to window");

			if (events.take_screenshot)
			{
				// Calculate filename
				const auto p1 = std::chrono::system_clock::now();
				int64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(p1.time_since_epoch()).count();
				// Save screenshot
				r.save_screenshot(std::to_string(ms) + ".ppm");
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
			total_time_seconds += timer.delta_time_seconds;
			benchmark.recordFrameTime(timer.delta_time_seconds);
		} while (running);

		benchmark.stop();
	}

};



