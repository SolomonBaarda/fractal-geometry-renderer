#pragma once

#include "Vector3.h"

#include <cstdint>
#include <vector>
#include <utility>
#include <cmath>

class Scene
{
private:
	uint32_t current_position_index = 0, current_facing_index = 0;

	static bool compare_vector_with_time(const std::pair<Vector3, float>& a, const std::pair<Vector3, float>& b)
	{
		return a.second < b.second;
	}

	static bool remove_if_time_is_invalid(const std::pair<Vector3, float>& a)
	{
		return a.second < 0;
	}

	static float lerp(float a, float b, float t)
	{
		return a + t * (b - a);
	}

	static Vector3 lerp(const Vector3& a, const Vector3& b, float t)
	{
		return Vector3::add(a, (b - a) * t);
	}

public:
	Scene(Vector3 camera_up_axis, std::vector <std::pair<Vector3, float>> camera_positions_at_time,
		std::vector <std::pair<Vector3, float>> camera_facing_directions_at_time, bool do_camera_loop) :
		camera_up_axis(camera_up_axis), camera_positions_at_time(camera_positions_at_time),
		camera_facing_directions_at_time(camera_facing_directions_at_time), do_camera_loop(do_camera_loop), allow_user_camera_control(true)
	{
		camera_up_axis.normalise();

		// Remove invalid elements from the vectors
		camera_positions_at_time.erase(std::remove_if(camera_positions_at_time.begin(), camera_positions_at_time.end(), remove_if_time_is_invalid), camera_positions_at_time.end());
		camera_facing_directions_at_time.erase(std::remove_if(camera_facing_directions_at_time.begin(), camera_facing_directions_at_time.end(), remove_if_time_is_invalid), camera_facing_directions_at_time.end());

		// Sort them so they occur in order of time
		std::sort(camera_positions_at_time.begin(), camera_positions_at_time.end(), compare_vector_with_time);
		std::sort(camera_facing_directions_at_time.begin(), camera_facing_directions_at_time.end(), compare_vector_with_time);

		// Add default values if the vectors are empty
		if (camera_positions_at_time.empty())
		{
			camera_positions_at_time.push_back(std::pair(Vector3(0.0f, 0.0f, 0.0f), 0.0f));
		}
		if (camera_facing_directions_at_time.empty())
		{
			camera_facing_directions_at_time.push_back(std::pair(Vector3(1.0f, 0.0f, 0.0f), 0.0f));
		}

		allow_user_camera_control = camera_positions_at_time.size() == 1 && camera_facing_directions_at_time.size() == 1;


		printf("Camera positions at time for scene: [");
		for (const auto& a : camera_positions_at_time)
		{
			printf("(%f, %f, %f) @ %f, ", a.first.x, a.first.y, a.first.z, a.second);
		}
		printf("]\n");

		printf("Camera facing directions at time for scene: [");
		for (const auto& a : camera_facing_directions_at_time)
		{
			printf("(%f, %f, %f) @ %f, ", a.first.x, a.first.y, a.first.z, a.second);
		}
		printf("]\n");
	}

	Vector3 camera_up_axis;

	std::vector <std::pair<Vector3, float>> camera_positions_at_time;
	std::vector <std::pair<Vector3, float>> camera_facing_directions_at_time;

	bool allow_user_camera_control;
	bool do_camera_loop;


	Vector3 get_camera_value_at_time(const std::vector <std::pair<Vector3, float>> vector, float time, bool do_loop)
	{
		if (vector.size() == 1)
		{
			return vector.at(0).first;
		}
		else
		{
			// Get the time relative to 0 and the max for this camera path
			float time_relative = do_loop ? fmod(time, vector.back().second) : time;

			// Before the first element
			if (time_relative < vector.front().second)
			{
				return vector.front().first;
			}
			// After the last element
			else if (time_relative > vector.back().second)
			{
				return vector.back().first;
			}
			// Otherwise, we must be somewhere between
			else
			{
				for (uint32_t i = 0; i < vector.size() - 1; i++)
				{
					const auto& before = vector.at(i);
					const auto& after = vector.at(i + 1);

					// Current time is somewhere between these two values
					if (before.second <= time_relative && after.second > time_relative)
					{
						return lerp(before.first, after.first, (time_relative - before.second) / (after.second - before.second));
					}
				}
			}


		}
	}
};
