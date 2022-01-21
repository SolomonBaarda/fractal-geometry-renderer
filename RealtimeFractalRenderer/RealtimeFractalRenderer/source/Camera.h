#pragma once

#define _USE_MATH_DEFINES

#include <cmath>
#include <algorithm>

#include "Events.h"
#include "Vector3.h"

#define TO_RADIANS M_PI / 180.0f
#define TO_DEGREES 180.0f / M_PI

class Camera
{
private:
	Vector3 up;
public:
	Camera()
	{ }

	Camera(Vector3 up) : up(up)
	{
		up.normalise();
	}

	Vector3 position, facing;

	float speed = 50.0f;
	float sprint_multiplier = 3.0f;

	float sensitivity = 30.0f;
	float yaw = 0.0f, pitch = 0.0f;

	void update(Events e, float delta_time)
	{
		facing.normalise();

		// Update facing direction first

		if (e.mouse_within_window)
		{
			const float sens = sensitivity * delta_time;

			yaw += e.delta_mouse_x * sens;
			pitch += -e.delta_mouse_y * sens;
			pitch = std::clamp(pitch, -89.0f, 89.0f);
		}

		const float yaw_radians = yaw * TO_RADIANS, pitch_radians = pitch * TO_RADIANS;

		// Update camera direction
		facing.x = cos(yaw_radians) * cos(pitch_radians);
		facing.y = sin(pitch_radians);
		facing.z = sin(yaw_radians) * cos(pitch_radians);
		facing.normalise();

		// Then update position

		const float delta_v = e.sprint ? sprint_multiplier * speed * delta_time : speed * delta_time;

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







	// Update camera direction
	//facing.x = cos(yaw_radians) * cos(pitch_radians);
	//facing.y = sin(pitch_radians);
	//facing.z = sin(yaw_radians) * cos(pitch_radians);






	static void calculatePitchAndYaw(const Vector3& facing, float* pitch, float* yaw)
	{
		float pitch_radians = asin(facing.y);
		*pitch = pitch_radians * TO_DEGREES;
		*yaw = acos(facing.x) / pitch_radians * TO_DEGREES;
	}



};