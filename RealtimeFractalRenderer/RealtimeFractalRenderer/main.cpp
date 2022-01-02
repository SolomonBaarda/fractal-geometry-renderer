#include "Renderer.h"
#include "Window.h"

#include <cstdint>
#include <cstdio>
#include <string>
#include <windows.h> // required for profileapi
#include <profileapi.h>

int main()
{
	LARGE_INTEGER frequency, t1, t2;
	double delta_time_seconds = 0, total_time_seconds = 0;
	// Get ticks per second
	QueryPerformanceFrequency(&frequency);

	const uint32_t width = 900, height = 600;

	Window w(width, height);
	Renderer r(width, height);

	r.load_kernel("../../../../RealTimeFractalRenderer/kernels/main.cl");


	// Flush any events that occured before now
	w.get_events();

	Camera camera;
	camera.position = Vector3(-10, -5, -10);
	// Facing vector from looking at position 0, 0, 0
	camera.facing = camera.position - Vector3(0, 0, 0);

	do
	{
		QueryPerformanceCounter(&t1); // START TIMER

		// Process events
		Events e = w.get_events();

		// Update objects in the scene
		camera.update(e, delta_time_seconds);

		// Render the scene
		r.render(camera, total_time_seconds);
		w.set_pixels(r.buffer);

		QueryPerformanceCounter(&t2); // END TIMER

		delta_time_seconds = ((t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart) / 1000;
		total_time_seconds += delta_time_seconds;

		printf("Frame time: %.1f ms / FPS: %.1f / Total time: %.1f s\n", delta_time_seconds * 1000.0, 1.0 / delta_time_seconds, total_time_seconds);
	} while (true);

	return 0;
}
