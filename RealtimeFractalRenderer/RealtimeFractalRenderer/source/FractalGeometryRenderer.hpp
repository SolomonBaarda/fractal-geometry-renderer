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
		r.load_kernel(scene_path, build_options);

		Timer t;
		Events events;
		Camera camera;

		camera.position = Vector3(-0.5f, -0.5f, -0.5f);
		//camera.position = Vector3(-10, -10, -10);

		// Facing vector from looking at position 0, 0, 0
		//Vector3 facing = (camera.position - Vector3(0, 0, 0)).normalise();

		//camera.pitch = -35.0f;
		//camera.yaw = -133.9f;

		// Flush any events that occured before now
		w.get_events();

		bool running = true;
		float total_time_seconds = 0;

		Benchmark b("Total frame time");
		b.start();

		printf("\n");

		do
		{
			t.start();

			b.addMarkerNow("start of frame");

			// Process events
			events = w.get_events();
			running = !events.exit;

			b.addMarkerNow("poll events");

			// Update objects in the scene
			camera.update(events, t.delta_time_seconds);
			//printf("Camera pos: (%.1f, %.1f, %.1f) pitch: %.1f yaw: %.1f facing: (%.1f, %.1f, %.1f)\n", camera.position.x, camera.position.y, camera.position.z, camera.pitch, camera.yaw, camera.facing.x, camera.facing.y, camera.facing.z);
			b.addMarkerNow("update camera");

			// Render the scene
			r.render(camera, total_time_seconds);
			b.addMarkerNow("render to buffer");

			w.set_pixels(r.buffer);
			b.addMarkerNow("render buffer to window");

			if (events.take_screenshot)
			{
				// Calculate filename
				const auto p1 = std::chrono::system_clock::now();
				int64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(p1.time_since_epoch()).count();
				// Save screenshot
				r.save_screenshot(std::to_string(ms) + ".ppm");
			}
			b.addMarkerNow("take screenshot");

			// Must be the last lines of the main loop
			t.stop();
			total_time_seconds += t.delta_time_seconds;
			b.recordFrameTime(t.delta_time_seconds);
		} while (running);

		b.stop();
	}

};



