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

		template <typename T>
		struct Compare
		{
			bool operator()(const std::pair<T, float>& a, const std::pair<T, float>& b) const { return a.second < b.second; }
		};

		static bool remove_if_time_is_invalid_v(const std::pair<Eigen::Vector3f, float>& a)
		{
			return a.second < 0;
		}

		static bool remove_if_time_is_invalid_q(const std::pair<Eigen::Quaternionf, float>& a)
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

		}

		template<class T>
		std::tuple<T, T, float> get_camera_value_at_time(const std::vector <std::pair<T, float>>& vector, float time, bool do_loop)
		{
			if (vector.size() == 1)
			{
				return std::tuple(vector.at(0).first, vector.at(0).first, vector.at(0).second);
			}
			else
			{
				// Get the time relative to 0 and the max for this camera path
				float time_relative = do_loop ? fmod(time, vector.back().second) : time;

				// Before the first element
				if (time_relative < vector.front().second)
				{
					return std::tuple(vector.front().first, vector.front().first, vector.front().second);
				}
				// After the last element
				else if (time_relative > vector.back().second)
				{
					return std::tuple(vector.back().first, vector.back().first, vector.back().second);
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
							return std::tuple(before.first, after.first, (time_relative - before.second) / (after.second - before.second));
						}
					}
				}
			}
		}

	public:
		Eigen::Vector3f camera_up_axis;

		std::vector <std::pair<Eigen::Vector3f, float>> camera_positions_at_time;
		std::vector <std::pair<Eigen::Quaternionf, float>> camera_rotations_at_time;

		bool allow_user_camera_control, do_camera_loop;

		std::pair<float, float> benchmark_start_stop_time;
		bool do_timed_benchmark;

		Scene(Eigen::Vector3f camera_up_axis, std::vector <std::pair<Eigen::Vector3f, float>> camera_positions_at_time,
			std::vector <std::pair<Eigen::Vector3f, float>> camera_facing_directions_at_time, bool do_camera_loop,
			std::pair<float, float> benchmark_start_stop_time) :

			camera_up_axis(camera_up_axis), camera_positions_at_time(camera_positions_at_time),
			do_camera_loop(do_camera_loop), camera_rotations_at_time(),
			allow_user_camera_control(true), benchmark_start_stop_time(benchmark_start_stop_time)
		{
			camera_up_axis.normalize();

			// Convert all facing directions to quaternions
			for (auto& facing : camera_facing_directions_at_time)
			{
				Eigen::Quaternionf rotation = Eigen::Quaternionf::FromTwoVectors(Eigen::Vector3f(0, 0, 1), facing.first);
				camera_rotations_at_time.push_back(std::pair(rotation, facing.second));
			}


			// Remove invalid elements from the vectors
			camera_positions_at_time.erase(std::remove_if(camera_positions_at_time.begin(), camera_positions_at_time.end(), remove_if_time_is_invalid_v), camera_positions_at_time.end());
			camera_rotations_at_time.erase(std::remove_if(camera_rotations_at_time.begin(), camera_rotations_at_time.end(), remove_if_time_is_invalid_q), camera_rotations_at_time.end());


			// Sort them so they occur in order of time
			std::sort(camera_positions_at_time.begin(), camera_positions_at_time.end(), Compare<Eigen::Vector3f>());
			std::sort(camera_rotations_at_time.begin(), camera_rotations_at_time.end(), Compare<Eigen::Quaternionf>());

			// Add default values if the vectors are empty
			if (camera_positions_at_time.empty())
			{
				camera_positions_at_time.push_back(std::pair(Eigen::Vector3f(0.0f, 0.0f, 0.0f), 0.0f));
			}
			if (camera_rotations_at_time.empty())
			{
				camera_rotations_at_time.push_back(std::pair(Eigen::Quaternionf(), 0.0f));
			}

			allow_user_camera_control = camera_positions_at_time.size() == 1 && camera_rotations_at_time.size() == 1;

			do_timed_benchmark = benchmark_start_stop_time.first >= 0 && benchmark_start_stop_time.second >= benchmark_start_stop_time.first;
		}


		Eigen::Vector3f get_camera_position_at_time(float time)
		{
			const auto& position = get_camera_value_at_time<Eigen::Vector3f>(camera_positions_at_time, time, do_camera_loop);
			return lerp(std::get<0>(position), std::get<1>(position), std::get<2>(position));
		}

		Eigen::Vector3f get_camera_facing_direction_at_time(float time)
		{
			const auto& rotations = get_camera_value_at_time<Eigen::Quaternionf>(camera_rotations_at_time, time, do_camera_loop);
			Eigen::Quaternionf rotation = std::get<0>(rotations).slerp(std::get<2>(rotations), std::get<1>(rotations));
			return (rotation * Eigen::Vector3f(0, 0, 1)).normalized();
		}

	};
}
