#include "Renderer.h"

//// for initializing and shutdown functions
//#include <SDL2/SDL.h>
//
//// for rendering images and graphics on screen
//#include <SDL2/SDL_image.h>
//
//// for using SDL_Delay() functions
//#include <SDL2/SDL_timer.h>











int main(int argc, char* argv[])
{
	const float aspectRatio = 16.0f / 9.0f;
	const int32_t width = 1920, height = static_cast<int>(static_cast<float>(width) / aspectRatio);

	Renderer r(width, height, aspectRatio);

	r.render();
	r.saveToFile("image.ppm");



	return 0;
}

