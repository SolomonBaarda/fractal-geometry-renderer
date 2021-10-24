#include <SDL2/SDL.h>
//#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_timer.h>

#include "Renderer.h"
#include "Window.h"




int main(int argc, char* argv[])
{
	const float aspectRatio = 16.0f / 9.0f;
	const int32_t width = 1920, height = static_cast<int>(static_cast<float>(width) / aspectRatio);

	Window window(width, height);
	Renderer renderer(width, height, aspectRatio);


	renderer.render();


	char* pixels = new char[width * height * 3];
	for (int32_t i = 0; i < width * height; i++)
	{
		int32_t index = i * 3;
		pixels[index] = renderer.buffer[i].x;
		pixels[index + 1] = renderer.buffer[i].y;
		pixels[index + 2] = renderer.buffer[i].z;
	}

	window.setPixels(renderer.buffer);
	renderer.saveToFile("image.ppm");


	SDL_Delay(3000);  // Pause execution for 3000 milliseconds, for example

	return 0;
}

