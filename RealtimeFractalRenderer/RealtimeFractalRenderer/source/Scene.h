#pragma once

#include "Vector3.h"

#include <cstdint>
#include <vector>
#include <utility>

class Scene
{
public:
	float camera_vertical_fov, camera_focus_distance;
	Vector3 camera_up_axis;

	std::vector <std::pair<Vector3, float>> camera_positions_at_time;
	std::vector <std::pair<Vector3, float>> camera_facing_directions_at_time;

	bool do_camera_loop;
};
