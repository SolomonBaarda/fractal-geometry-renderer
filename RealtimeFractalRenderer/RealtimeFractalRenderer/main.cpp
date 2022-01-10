#include "Renderer.h"
#include "Window.h"
#include "Benchmark.h"

#include "Timer.h"

#include <cstdint>
#include <cstdio>
#include <string>


int main()
{
	const uint32_t width = 1920, height = 1080;

	Window w(width, height);
	Renderer r(width, height);
	Timer t;
	Benchmark b;
	Camera camera;
	Events events;

	r.load_kernel("../../../../RealTimeFractalRenderer/kernels/sphere-box.cl");

	camera.position = Vector3(-10, -5, -10);
	// Facing vector from looking at position 0, 0, 0
	camera.facing = camera.position - Vector3(0, 0, 0);

	// Flush any events that occured before now
	w.get_events();

	do
	{
		t.start();

		// Process events
		events = w.get_events();

		// Update objects in the scene
		camera.update(events, t.delta_time_seconds);
		printf("Mouse delta: %d %d \n", events.delta_mouse_x, events.delta_mouse_y);
		printf("Camera facing: %.1f %.1f %.1f\n", camera.facing.x, camera.facing.y, camera.facing.z);

		// Render the scene
		r.render(camera, t.total_time_seconds);
		w.set_pixels(r.buffer);

		t.stop();
		b.record_frame(t.delta_time_seconds);

		printf("Frame time: %.1f ms / FPS: %.1f / Total time: %.1f s\n", t.delta_time_seconds * 1000.0, 1.0 / t.delta_time_seconds, t.total_time_seconds);
	} while (true);

	return 0;
}
