#pragma once

#include <windows.h> // required for profileapi
#include <profileapi.h>

class Timer
{
private:
	LARGE_INTEGER frequency, before, after;
public:
	float delta_time_seconds = 0, total_time_seconds = 0;

	Timer()
	{
		// Get ticks per second
		QueryPerformanceFrequency(&frequency);
	}

	void start()
	{
		QueryPerformanceCounter(&before);
	}

	void stop()
	{
		QueryPerformanceCounter(&after);

		delta_time_seconds = ((after.QuadPart - before.QuadPart) * 1000.0 / frequency.QuadPart) / 1000;
		total_time_seconds += delta_time_seconds;
	}

};