#include <SDL2/SDL.h>
#include <chrono>
#include <string>

#include "Renderer.h"
#include "Window.h"
#include "Controller.h"


int main(int argc, char* argv[])
{
	std::chrono::steady_clock clock;
	std::chrono::steady_clock::time_point before;

	const float aspectRatio = 16.0f / 9.0f;
	const int32_t width = 1920, height = static_cast<int>(static_cast<float>(width) / aspectRatio);

	Window window(width, height);
	Renderer renderer(width, height, aspectRatio);
	Controller controller;


	//while (true)
	{
		before = clock.now();

		controller.handleInputs();

		renderer.render();
		window.setPixels(renderer.buffer);

		double duration = std::chrono::duration_cast<std::chrono::milliseconds> (clock.now() - before).count();

		std::cout << "frame time: " << std::to_string(duration) << "ms";
	}

	renderer.saveToFile("image.ppm");


	SDL_Delay(2000);  // Pause execution for 3000 milliseconds, for example

	return 0;
}

