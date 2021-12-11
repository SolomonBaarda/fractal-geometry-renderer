#include <cstdint>
#include "Controller.h"
#include "Renderer.h"
#include "Display.h"



int main()
{
	const uint32_t width = 900, height = 600;

	Controller c;
	Display d(width, height);
	Renderer r(width, height);

	r.render();
	//d.saveToFile(r.buffer, width, height, "file.ppm");
	d.set_pixels(r.buffer);

	std::cout << "rendered";

	while (true)
	{
		d.poll_events();
	}

	return 0;
}
