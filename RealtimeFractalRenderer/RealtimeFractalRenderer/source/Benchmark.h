#pragma once

#include <cstdint>
#include <limits>

class Benchmark
{
public:
	uint64_t number_of_frames = 0;
	float total_frame_time = 0, minimum_frame_time, maximum_frame_time;

	Benchmark() : minimum_frame_time(std::numeric_limits<float>::max()), maximum_frame_time(std::numeric_limits<float>::min())
	{ }

	void record_frame(float frame_time)
	{
		number_of_frames++;
		total_frame_time += frame_time;

		if (frame_time < minimum_frame_time)
			minimum_frame_time = frame_time;

		if (frame_time > maximum_frame_time)
			maximum_frame_time = frame_time;
	}
};