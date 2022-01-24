#pragma once

#define SDL_MAIN_HANDLED

#include "Vector3.h"
#include "Events.h"
#include "Benchmark.h"

#include <SDL.h>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cstring>


/// <summary>
/// 
/// </summary>
class Window
{
public:
	Window();
	Window(uint32_t width, uint32_t height);
	~Window();

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	Events get_events();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="pixels"></param>
	void set_pixels(uint8_t* pixels);

private:
	const uint32_t width, height;
	const int32_t middle_x = width / 2, middle_y = height / 2;

	uint8_t* colours;

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_RendererInfo info;
	SDL_Texture* texture;
	SDL_Event event;

	Events events_since_last_get;
	SDL_bool capture_mouse = SDL_FALSE;

	Benchmark b;

	inline float clamp01(float x)
	{
		return x < 0 ? 0 : x > 1 ? 1 : x;
	}

	inline uint8_t toInt(float x)
	{
		return static_cast<uint8_t>(clamp01(x) * 255);
		// Applies a gamma correction of 2.2
		return static_cast<uint8_t>(pow(clamp01(x), 1 / 2.2) * 255 + .5);
	}
};
