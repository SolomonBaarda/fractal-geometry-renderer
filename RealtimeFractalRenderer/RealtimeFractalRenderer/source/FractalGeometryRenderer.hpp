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
	Benchmark b;
	Timer t;
	Events events;

	Camera camera;

	Window w;
	Renderer r;

public:
	FractalGeometryRenderer() : FractalGeometryRenderer(1920, 1080)
	{ }


	FractalGeometryRenderer(uint32_t width, uint32_t height) : w(width, height), r(width, height)
	{
	}

	~FractalGeometryRenderer()
	{
		//float average_fps = b.number_of_frames / b.total_frame_time;
		//printf("Average FPS: %.1f", average_fps);
		b.printResultsToConsole();
	}

	void run()
	{
		r.load_kernel("../../../../RealTimeFractalRenderer/kernels/sphere_box.cl", "-I ../../../../RealTimeFractalRenderer/kernels/include");

		// pos: 0.1, -32.2, -27.4 facing: 0, -1, 0
		// pos: -17.3, -25.9, -25.9 facing: 0.9, -0.1, -0.4

		camera.position = Vector3(0.1, -32.2, -27.4);
		// Facing vector from looking at position 0, 0, 0
		//camera.facing = camera.position - Vector3(0, 0, 0);
		camera.facing = Vector3(0, -1, 0);

		// Flush any events that occured before now
		w.get_events();

		bool running = true;
		float total_time_seconds = 0;

		b.start();
		do
		{
			t.start();

			b.addMarkerNow("start of frame");

			// Process events
			events = w.get_events();

			b.addMarkerNow("poll events");

			// Update objects in the scene
			camera.update(events, t.delta_time_seconds);
			//printf("Camera pos: %.1f %.1f %.1f facing: %.1f %.1f %.1f\n", camera.position.x, camera.position.y, camera.position.z, camera.facing.x, camera.facing.y, camera.facing.z);
			b.addMarkerNow("update camera");

			// Render the scene
			r.render(camera, total_time_seconds);
			b.addMarkerNow("render to buffer");

			w.set_pixels(r.buffer);
			b.addMarkerNow("render buffer to window");

			if (events.take_screenshot)
			{
				const auto p1 = std::chrono::system_clock::now();
				int64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(p1.time_since_epoch()).count();

				r.save_screenshot(std::to_string(ms) + ".ppm");
			}
			b.addMarkerNow("take screenshot");

			if (events.exit)
			{
				running = false;
			}

			// Must be the last lines of the main loop
			t.stop();
			total_time_seconds += t.delta_time_seconds;

			//printf("Frame time: %.1f ms / FPS: %.1f / Total time: %.1f s\n", t.delta_time_seconds * 1000.0, 1.0 / t.delta_time_seconds, t.total_time_seconds);
		} while (running);

		b.stop();
	}

};


