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
		float clock_frequency = 0, before = 0, last_delta_time_seconds = 0, current_delta_time_seconds = 0;
	public:

		Timer()
		{
			SDL_Init(SDL_INIT_TIMER);
			clock_frequency = static_cast<float>(SDL_GetPerformanceFrequency());
		}

		/// <summary>
		/// </summary>
		/// <returns>The time between the last start and stop calls in seconds</returns>
		float getLastDeltaTimeSeconds()
		{
			return last_delta_time_seconds;
		}

		/// <summary>
		/// Starts the timer. If this is called multiple times it will overwrite the starting 
		/// time with the most revent value.
		/// </summary>
		void start()
		{
			before = static_cast<float>(SDL_GetPerformanceCounter());
			current_delta_time_seconds = 0.0f;
		}

		/// <summary>
		/// Pauses the timer so that it can be resumed later on.
		/// </summary>
		void pause()
		{
			float after = static_cast<float>(SDL_GetPerformanceCounter());
			current_delta_time_seconds += (after - before) / clock_frequency;
		}

		/// <summary>
		/// Continues the timer without resetting the getLastDeltaTimeSeconds value.
		/// </summary>
		void resume()
		{
			before = static_cast<float>(SDL_GetPerformanceCounter());
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