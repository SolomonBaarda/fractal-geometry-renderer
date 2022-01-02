#pragma once

#define SDL_MAIN_HANDLED

#include "Vector3.h"

#include "SDL.h"
#include <cstdint>
#include <iostream>
#include "Events.h"

#include <iomanip>
#include <vector>
#include <cstring>



class Window
{
public:
	Window();
	Window(uint32_t width, uint32_t height);
	~Window();

	Events get_events();
	void set_pixels(uint8_t * pixels);

private:
	uint32_t width, height;
	int32_t middle_x = width / 2, middle_y = height / 2;

	uint8_t* colours;

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_RendererInfo info;
	SDL_Texture* texture;
	SDL_Event event;

	Events events_since_last_get;
	SDL_bool capture_mouse = SDL_FALSE;

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

public:
	void saveToFile(Vector3* image, const int32_t width, const int32_t height, const char* filename)
	{
		FILE* f = fopen(filename, "w"); // Write image to PPM file.
		fprintf(f, "P3\n%d %d\n%d\n", width, height, 255);

		// Rows
		for (int32_t y = 0; y < height; y++)
		{		// Columns
			for (int32_t x = 0; x < width; x++)
			{
				int32_t index = y * width + x;
				fprintf(f, "%d %d %d ", toInt(image[index].x), toInt(image[index].y), toInt(image[index].z));
			}

		}

		fclose(f);
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


