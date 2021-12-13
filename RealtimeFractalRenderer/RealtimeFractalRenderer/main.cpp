#include "Controller.h"
#include "Renderer.h"
#include "Display.h"

#include <cstdint>
#include <string>
#include <windows.h> // required for profileapi
#include <profileapi.h>

int main()
{
	LARGE_INTEGER frequency, t1, t2;
	// Get ticks per second
	QueryPerformanceFrequency(&frequency);

	const uint32_t width = 900, height = 600;

	Controller c;
	Display d(width, height);
	Renderer r(width, height);


	QueryPerformanceCounter(&t1); // START TIMER

	//r.render();
	//d.saveToFile(r.buffer, width, height, "file.ppm");
	//d.set_pixels(r.buffer);

	QueryPerformanceCounter(&t2); // END TIMER

	// Compute elapsed time in milliseconds
	double elapsed_time_ms = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;

	std::cout << "Rendered and displayed frame in " << std::to_string(elapsed_time_ms) << " milliseconds\n";

	std::cout << "running kernel code\n";
	r.run();
	std::cout << "ran kernel code\n";

	exit(0);

	while (true)
	{
		d.poll_events();
	}

	return 0;
}
