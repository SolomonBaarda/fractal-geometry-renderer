#pragma once

#include "Timer.h"

#include <string>
#include <cstdint>
#include <vector>
#include <limits>

struct BenchmarkMarker
{
	std::string description;
	float total_seconds;
	uint64_t number_of_occurrences;

	BenchmarkMarker() : description(""), total_seconds(0.0f), number_of_occurrences(0u)
	{ }
};

class Benchmark
{
private:
	std::string description;

	Timer t;
	bool isRunning = false;

	float total_frame_time_seconds = 0, minimum_frame_time_seconds = 0, maximum_frame_time_seconds = 0;
	uint32_t total_number_frames = 0;

	std::vector<BenchmarkMarker> markers;
	uint32_t current_index = 0;

public:
	Benchmark() : Benchmark("")
	{ }

	Benchmark(std::string description) : description(description)
	{ }

	~Benchmark()
	{
		printResultsToConsole();
	}

	bool getIsRunning()
	{
		return isRunning;
	}

	void start()
	{
		current_index = 0;
		total_frame_time_seconds = 0;
		minimum_frame_time_seconds = std::numeric_limits<float>::max();
		maximum_frame_time_seconds = std::numeric_limits<float>::min();
		total_number_frames = 0u;
		markers.clear();

		isRunning = true;
		t.start();
	}

	void stop()
	{
		isRunning = false;
	}

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
			marker.total_seconds += t.delta_time_seconds;
			markers.at(current_index) = marker;

			// Increment the current index and start the timer again
			current_index++;
			t.start();
		}
	}

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

	void printResultsToConsole()
	{
		printf("\nResults for benchmark %s:\n", description.c_str());

		if (total_number_frames > 0)
		{
			printf("\tTotal time: %f seconds\n", total_frame_time_seconds);
			printf("\tTotal number of frames: %u\n", total_number_frames);
			float min_frame_time_ms = minimum_frame_time_seconds * 1000.0f;
			printf("\tMinimum frame time: %f ms\n", min_frame_time_ms);
			float max_frame_time_ms = maximum_frame_time_seconds * 1000.0f;
			printf("\tMaximum frame time: %f ms\n", max_frame_time_ms);
			float average_frame_time = total_frame_time_seconds / static_cast<float>(total_number_frames) * 1000.0f;
			printf("\tAverage frame time: %f ms\n", average_frame_time);
			float average_fps = static_cast<float>(total_number_frames) / total_frame_time_seconds;
			printf("\tAverage FPS: %f\n", average_fps);
			printf("\n");
		}

		if (markers.size() > 0)
		{
			printf("\tAverage frame time breakdown:\n");
			for (uint32_t i = 0; i < markers.size(); i++)
			{
				uint32_t next_index = i == markers.size() - 1 ? 0 : i + 1;
				float average_ms = markers.at(next_index).total_seconds / markers.at(next_index).number_of_occurrences * 1000.0f;
				printf("\t\t%s -> %s: %f ms\n", markers.at(i).description.c_str(), markers.at(next_index).description.c_str(), average_ms);
			}
			printf("\n");
		}
	}
};

