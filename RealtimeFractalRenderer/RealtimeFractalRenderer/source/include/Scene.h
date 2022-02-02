#pragma once

#include <cstdint>
#include <vector>
#include <utility>
#include <cmath>
#include <Eigen/Geometry>

namespace FractalGeometryRenderer
{
	/// <summary>
	/// A class containing information about the current scene.
	/// </summary>
	class Scene
	{
	private:
		uint32_t current_position_index = 0, current_facing_index = 0;

		static bool compare_vector_with_time(const std::pair<Eigen::Vector3f, float>& a, const std::pair<Eigen::Vector3f, float>& b)
		{
			return a.second < b.second;
		}

		static bool remove_if_time_is_invalid(const std::pair<Eigen::Vector3f, float>& a)
		{
			return a.second < 0;
		}

		static float lerp(float a, float b, float t)
		{
			return a + t * (b - a);
		}

		static Eigen::Vector3f lerp(const Eigen::Vector3f& a, const Eigen::Vector3f& b, float t)
		{
			return a + (b - a) * t;
		}

		void fixData()
		{
			camera_up_axis.normalize();

			// Remove invalid elements from the vectors
			camera_positions_at_time.erase(std::remove_if(camera_positions_at_time.begin(), camera_positions_at_time.end(), remove_if_time_is_invalid), camera_positions_at_time.end());
			camera_facing_directions_at_time.erase(std::remove_if(camera_facing_directions_at_time.begin(), camera_facing_directions_at_time.end(), remove_if_time_is_invalid), camera_facing_directions_at_time.end());

			// Sort them so they occur in order of time
			std::sort(camera_positions_at_time.begin(), camera_positions_at_time.end(), compare_vector_with_time);
			std::sort(camera_facing_directions_at_time.begin(), camera_facing_directions_at_time.end(), compare_vector_with_time);

			// Add default values if the vectors are empty
			if (camera_positions_at_time.empty())
			{
				camera_positions_at_time.push_back(std::pair(Eigen::Vector3f(0.0f, 0.0f, 0.0f), 0.0f));
			}
			if (camera_facing_directions_at_time.empty())
			{
				camera_facing_directions_at_time.push_back(std::pair(Eigen::Vector3f(1.0f, 0.0f, 0.0f), 0.0f));
			}

			allow_user_camera_control = camera_positions_at_time.size() == 1 && camera_facing_directions_at_time.size() == 1;

			do_timed_benchmark = benchmark_start_stop_time.first >= 0 && benchmark_start_stop_time.second >= benchmark_start_stop_time.first;
		}

	public:
		Scene(Eigen::Vector3f camera_up_axis, std::vector <std::pair<Eigen::Vector3f, float>> camera_positions_at_time,
			std::vector <std::pair<Eigen::Vector3f, float>> camera_facing_directions_at_time, bool do_camera_loop,
			std::pair<float, float> benchmark_start_stop_time) :

			camera_up_axis(camera_up_axis), camera_positions_at_time(camera_positions_at_time),
			camera_facing_directions_at_time(camera_facing_directions_at_time), do_camera_loop(do_camera_loop),
			allow_user_camera_control(true), benchmark_start_stop_time(benchmark_start_stop_time)
		{
			fixData();
		}

		Eigen::Vector3f camera_up_axis;

		std::vector <std::pair<Eigen::Vector3f, float>> camera_positions_at_time;
		std::vector <std::pair<Eigen::Vector3f, float>> camera_facing_directions_at_time;

		bool allow_user_camera_control, do_camera_loop;

		std::pair<float, float> benchmark_start_stop_time;
		bool do_timed_benchmark;

		Eigen::Vector3f get_camera_value_at_time(const std::vector <std::pair<Eigen::Vector3f, float>> vector, float time, bool do_loop)
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
}