#pragma once

#define _USE_MATH_DEFINES

#include <cmath>
#include <algorithm>

#include "Events.h"
#include "Vector3.h"

class Camera
{
private:
	const double to_radians = M_PI / 180.0;
public:
	Vector3 position, facing, up = Vector3(0, 1, 0);

	float vertical_fov = 40.0f;
	float foucs_distance = 0.1f;

	float speed = 50.0f;

	float sensitivity = 30.0f;
	float yaw = 0.0f, pitch = 0.0f;

	void update(Events e, float delta_time)
	{
		facing.normalise();
		up.normalise();

		// Update facing direction first

		if (e.mouse_within_window)
		{
			const float sens = sensitivity * delta_time;

			yaw += e.delta_mouse_x * sens;
			pitch -= e.delta_mouse_y * sens;
			pitch = std::clamp(pitch, -89.0f, 89.0f);
		}

		const float yaw_radians = yaw * to_radians, pitch_radians = pitch * to_radians;

		// Update camera direction
		facing.x = cos(yaw_radians) * cos(pitch_radians);
		facing.y = sin(pitch_radians);
		facing.z = sin(yaw_radians) * cos(pitch_radians);
		facing.normalise();


		// Then update position

		const float delta_v = speed * delta_time;

		if (e.forward)
			position = position - facing * delta_v;

		if (e.backward)
			position = position + facing * delta_v;

		if (e.left)
			position = position + Vector3::crossProduct(position, up).normalised() * delta_v;

		if (e.right)
			position = position - Vector3::crossProduct(position, up).normalised() * delta_v;

		if (e.up)
			position = position + up * delta_v;

		if (e.down)
			position = position - up * delta_v;

	}
};