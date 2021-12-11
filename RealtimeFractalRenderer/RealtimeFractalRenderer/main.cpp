#include <cstdint>
#include "Controller.h"
#include "Renderer.h"
#include "Display.h"

int main()
{
	const uint32_t width = 900, height = 600;

	Controller c;
	Display d(width, width);
	Renderer r(width, width);

	r.render();
	d.set_pixels(r.buffer);

	std::cout << "rendered";

	while (true)
	{
		d.poll_events();
	}

	return 0;
}
