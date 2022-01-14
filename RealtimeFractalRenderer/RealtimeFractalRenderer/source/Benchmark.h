#pragma once

#include "Timer.h"

#include <cstdint>
#include <vector>

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
	Timer t;

	std::vector<BenchmarkMarker> markers;
	uint32_t current_index = 0;
	bool isRunning = false;

public:
	void start()
	{
		current_index = 0;
		markers.clear();
		isRunning = true;

		t.start();
	}

	void stop()
	{
		t.stop();
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

	void printResultsToConsole()
	{
		printf("\nBenchmark results:\n");

		for (uint32_t i = 0; i < markers.size(); i++)
		{
			uint32_t next_index = i == markers.size() - 1 ? 0 : i + 1;
			float average_ms = markers.at(next_index).total_seconds / markers.at(next_index).number_of_occurrences * 1000.0f;
			printf("\t%s -> %s: %f ms average\n", markers.at(i).description.c_str(), markers.at(next_index).description.c_str(), average_ms);
		}
		printf("\n");
	}
};

