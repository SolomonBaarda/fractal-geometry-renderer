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
	double elapsed_time_ms;
	// Get ticks per second
	QueryPerformanceFrequency(&frequency);

	const uint32_t width = 900, height = 600;

	Window w(width, height);
	Renderer r(width, height);

	r.load_kernel("../../../../RealTimeFractalRenderer/kernels/main.cl");

	double total_time_seconds = 0, estimated_fps;

	do
	{
		QueryPerformanceCounter(&t1); // START TIMER

		// Process events
		w.poll_events();
		w.get_events();

		// Update the scene

		// Render the scene
		r.render(total_time_seconds);
		w.set_pixels(r.buffer);

		QueryPerformanceCounter(&t2); // END TIMER

		elapsed_time_ms = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
		total_time_seconds += elapsed_time_ms / 1000;
		estimated_fps = 1000 / elapsed_time_ms;

		printf("Frame time: %.1f FPS: %.1f Total time: %.1f\n", elapsed_time_ms, estimated_fps, total_time_seconds);
	}
	while (true);

	return 0;
}
