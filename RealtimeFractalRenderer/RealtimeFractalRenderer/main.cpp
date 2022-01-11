#include "Renderer.h"
#include "Window.h"
#include "Benchmark.h"

#include "Timer.h"

#include <cstdint>
#include <cstdio>
#include <string>

#include <chrono>


int main()
{
	const uint32_t width = 1920, height = 1080;

	Window w(width, height);
	Renderer r(width, height);
	Timer t;
	Benchmark b;
	Camera camera;
	Events events;

	r.load_kernel("../../../../RealTimeFractalRenderer/kernels/mandelbulb.cl", "-I ../../../../RealTimeFractalRenderer/kernels/include");

	// pos: 0.1, -32.2, -27.4 facing: 0, -1, 0
	// pos: -17.3, -25.9, -25.9 facing: 0.9, -0.1, -0.4

	camera.position = Vector3(0.1, -32.2, -27.4);
	// Facing vector from looking at position 0, 0, 0
	//camera.facing = camera.position - Vector3(0, 0, 0);
	camera.facing = Vector3(0, -1, 0);

	// Flush any events that occured before now
	w.get_events();

	do
	{
		t.start();

		// Process events
		events = w.get_events();

		// Update objects in the scene
		camera.update(events, t.delta_time_seconds);
		printf("Camera pos: %.1f %.1f %.1f facing: %.1f %.1f %.1f\n", camera.position.x, camera.position.y, camera.position.z, camera.facing.x, camera.facing.y, camera.facing.z);

		// Render the scene
		r.render(camera, t.total_time_seconds);
		w.set_pixels(r.buffer);

		t.stop();
		b.record_frame(t.delta_time_seconds);

		if (events.take_screenshot)
		{
			const auto p1 = std::chrono::system_clock::now();
			int64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(p1.time_since_epoch()).count();

			r.save_screenshot(std::to_string(ms) + ".ppm");
		}

		printf("Frame time: %.1f ms / FPS: %.1f / Total time: %.1f s\n", t.delta_time_seconds * 1000.0, 1.0 / t.delta_time_seconds, t.total_time_seconds);
	} while (true);

	return 0;
}
