#pragma once

#include <SDL.h>

class Timer
{
private:
	float clock_frequency, before;
public:
	float delta_time_seconds = 0;

	Timer()
	{
		SDL_Init(SDL_INIT_TIMER);
		clock_frequency = static_cast<float>(SDL_GetPerformanceFrequency());
	}

	void start()
	{
		before = static_cast<float>(SDL_GetPerformanceCounter());
	}

	void stop()
	{
		float after = SDL_GetPerformanceCounter();
		delta_time_seconds = (after - before) / clock_frequency;
	}
};