#pragma once

#include "Timer.h"

#include <string>
#include <cstdint>
#include <vector>
#include <limits>
#include <iostream>

/// <summary>
/// Namsepace containing profiling utilities used by the FractalGeometryRenderer, such as benchmarking and timer tools.
/// </summary>
namespace Profiling
{
	/// <summary>
	/// A class used for benchmarking pieces of code.
	/// 
	/// After creating an instance of this class, the start 
	/// method must be called before calls to addMarkerNow and recordFrameTime will succeed. The stop method
	/// should be called when finished. 
	/// 
	/// This class will automatically call the method printResultsToConsole when its destructor is called.
	/// </summary>
	class Benchmark
	{
	private:
		/// <summary>
		/// Class for storing a marker, used by Benchmark. 
		/// </summary>
		struct BenchmarkMarker
		{
			std::string description;
			double total_seconds;
			uint64_t number_of_occurrences;

			BenchmarkMarker() : description(""), total_seconds(0.0), number_of_occurrences(0u)
			{ }
		};

		std::string description;

		Timer t;
		bool isRunning = false;

		std::vector<BenchmarkMarker> markers;
		uint32_t current_index = 0;

		std::ostream& log;

	public:
		Benchmark(std::string description, std::ostream& log) : description(description), log(log)
		{ }

		~Benchmark()
		{
			printResultsToConsole();
		}

		double total_frame_time_seconds = 0, minimum_frame_time_seconds = 0, maximum_frame_time_seconds = 0;
		uint32_t total_number_frames = 0;

		/// <returns>True if the application is running</returns>
		bool getIsRunning()
		{
			return isRunning;
		}

		/// <summary>
		/// Starts the benchmark. This allows calls to addMarkerNow and recordFrameTime to succeed.
		/// </summary>
		void start()
		{
			current_index = 0;
			total_frame_time_seconds = 0;
			minimum_frame_time_seconds = std::numeric_limits<double>::max();
			maximum_frame_time_seconds = std::numeric_limits<double>::min();
			total_number_frames = 0u;
			markers.clear();

			isRunning = true;
			t.start();
		}

		/// <summary>
		/// Stops the benchmark. This prevents calls to addMarkerNow and recordFrameTime from succeeding.
		/// </summary>
		void stop()
		{
			isRunning = false;
		}

		/// <summary>
		/// Adds a marker with description or increments a counter if the marker already exists.
		/// </summary>
		/// <param name="description"></param>
		void addMarkerNow(std::string description)
		{
			if (isRunning)
			{
				t.stop();

				if (current_index >= markers.size())
				{
					// This is the final element which has the same description as the first element
					if (current_index > 0 && current_index == markers.size() && markers.at(0).description.compare(description) == 0)
					{
						// Loop back to the start
						current_index = 0;
					}
					// This is a new element
					else
					{
						// Insert new element
						BenchmarkMarker m;
						m.description = description;

						markers.push_back(m);
					}
				}

				// Update the marker
				BenchmarkMarker marker = markers.at(current_index);
				marker.number_of_occurrences++;
				marker.total_seconds += t.getLastDeltaTimeSeconds();
				markers.at(current_index) = marker;

				// Increment the current index and start the timer again
				current_index++;
				t.start();
			}
		}

		/// <summary>
		/// Compares the specified frame time against the current minimum and maximum found, updating 
		/// them respectively. Also updates the total time and total number of frames occured.
		/// </summary>
		/// <param name="frame_time_seconds"></param>
		void recordFrameTime(float frame_time_seconds)
		{
			if (isRunning)
			{
				total_frame_time_seconds += frame_time_seconds;
				total_number_frames++;

				if (frame_time_seconds < minimum_frame_time_seconds)
					minimum_frame_time_seconds = frame_time_seconds;

				if (frame_time_seconds > maximum_frame_time_seconds)
					maximum_frame_time_seconds = frame_time_seconds;
			}
		}

		/// <summary>
		/// Prints the benchmark results to console.
		/// </summary>
		void printResultsToConsole()
		{
			log << "\nResults for benchmark " << description << ":\n";

			if (total_number_frames > 0)
			{
				log << "\tTotal time: " << total_frame_time_seconds << " seconds\n";
				log << "\tTotal number of frames: " << total_number_frames << "\n";
				double min_frame_time_ms = minimum_frame_time_seconds * 1000.0;
				log << "\tMinimum frame time: " << min_frame_time_ms << " ms\n";
				double max_frame_time_ms = maximum_frame_time_seconds * 1000.0;
				log << "\tMaximum frame time: " << max_frame_time_ms << " ms\n";
				double average_frame_time = total_frame_time_seconds / static_cast<double>(total_number_frames) * 1000.0;
				log << "\tAverage frame time: " << average_frame_time << " ms\n";
				double average_fps = static_cast<float>(total_number_frames) / total_frame_time_seconds;
				log << "\tAverage FPS: " << average_fps << "\n";
				log << "\n";
			}

			if (markers.size() > 0)
			{
				log << "\tAverage frame time breakdown:\n";
				for (uint32_t i = 0; i < markers.size(); i++)
				{
					uint32_t next_index = i == markers.size() - 1 ? 0 : i + 1;
					double average_ms = markers.at(next_index).total_seconds / markers.at(next_index).number_of_occurrences * 1000.0;
					log << "\t\t" << markers.at(i).description << " -> " << markers.at(next_index).description << ": " << average_ms << " ms\n";
				}
				log << "\n";
			}
		}
	};
}