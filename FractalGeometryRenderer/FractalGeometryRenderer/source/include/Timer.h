#pragma once

#define SDL_MAIN_HANDLED

#include <SDL.h>

namespace Profiling
{
	/// <summary>
	/// A class which measures the time between two intervals.
	/// </summary>
	class Timer
	{
	private:
		Uint64 before = 0, clock_frequency = 0;
		double last_delta_time_seconds = 0, current_delta_time_seconds = 0;
	public:

		Timer()
		{
			SDL_Init(SDL_INIT_TIMER);
			clock_frequency = SDL_GetPerformanceFrequency();
		}

		/// <summary>
		/// </summary>
		/// <returns>The time between the last start and stop calls in seconds</returns>
		double getLastDeltaTimeSeconds()
		{
			return last_delta_time_seconds;
		}

		/// <summary>
		/// Starts the timer. If this is called multiple times it will overwrite the starting 
		/// time with the most revent value.
		/// </summary>
		void start()
		{
			before = SDL_GetPerformanceCounter();
			current_delta_time_seconds = 0.0f;
		}

		/// <summary>
		/// Pauses the timer so that it can be resumed later on.
		/// </summary>
		void pause()
		{
			Uint64 now = SDL_GetPerformanceCounter();
			current_delta_time_seconds += static_cast<double>(now - before) / clock_frequency;
		}

		/// <summary>
		/// Continues the timer without resetting the getLastDeltaTimeSeconds value.
		/// </summary>
		void resume()
		{
			before = SDL_GetPerformanceCounter();
		}

		/// <summary>
		/// Stops the timer. This sets the value returned by getLastDeltaTimeSeconds.
		/// </summary>
		void stop()
		{
			pause();
			last_delta_time_seconds = current_delta_time_seconds;
		}
	};
}