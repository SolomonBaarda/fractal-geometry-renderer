#pragma once

#define SDL_MAIN_HANDLED

#include <cstdint>
#include "SDL.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <cstring>

#include "Vector3.h"

class Display
{
public:
	Display();
	Display(uint32_t width, uint32_t height);
	~Display();

	void poll_events();
	void set_pixels(Vector3 * colours);

private:
	uint32_t width, height;
	uint8_t* colours;

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_RendererInfo info;
	SDL_Texture* texture;
	SDL_Event event;

	inline float clamp01(float x)
	{
		return x < 0 ? 0 : x > 1 ? 1 : x;
	}

	inline uint8_t toInt(float x)
	{
		return static_cast<uint8_t>(clamp01(x)* 255);
		// Applies a gamma correction of 2.2
		return static_cast<uint8_t>(pow(clamp01(x), 1 / 2.2) * 255 + .5);
	}
};





//int main(int argc, char** argv)
//{
//
//
//	while (running)
//	{
//
//		
//	}
//
//
//
//	return 0;
//}


