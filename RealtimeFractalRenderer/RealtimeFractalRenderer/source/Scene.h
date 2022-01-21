#pragma once

#include "Vector3.h"

#include <cstdint>
#include <vector>
#include <utility>

class Scene
{
public:
	Vector3 camera_up_axis;

	std::vector <std::pair<Vector3, float>> camera_positions_at_time;
	std::vector <std::pair<Vector3, float>> camera_facing_directions_at_time;

	bool allow_user_camera_control;
	bool do_camera_loop;
};
