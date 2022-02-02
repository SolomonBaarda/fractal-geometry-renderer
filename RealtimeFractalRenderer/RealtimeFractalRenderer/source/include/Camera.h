#pragma once

#define _USE_MATH_DEFINES

#include <cmath>
#include <algorithm>
#include <Eigen/Geometry>

#include "Events.h"

#define TO_RADIANS M_PI / 180.0f
#define TO_DEGREES 180.0f / M_PI

namespace FractalGeometryRenderer
{
	/// <summary>
	/// A class for storing and controlling a camera's position and view direction within a scene.
	/// </summary>
	class Camera
	{
	private:
		Eigen::Vector3f up;
	public:
		Camera()
		{ }

		Camera(Eigen::Vector3f up) : up(up)
		{
			up.normalize();
		}

		/// <summary>
		/// The current position of the camera in world space.
		/// </summary>
		Eigen::Vector3f position;

		/// <summary>
		/// A normalised vector representing the direction that the camera is facing.
		/// </summary>
		Eigen::Vector3f facing;

		float speed = 5.0f;
		float sprint_multiplier = 3.0f;

		float sensitivity = 30.0f;
		float yaw = 0.0f, pitch = 0.0f;

		/// <summary>
		/// Updates the camera using the events specified. Use this method when using keyboard and mouse input.
		/// </summary>
		/// <param name="e">The keyboard and mouse events</param>
		/// <param name="delta_time">The time taken for last frame to execute fully</param>
		void update(Events e, float delta_time)
		{
			facing.normalize();

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
			facing = Eigen::Vector3f(cos(yaw_radians) * cos(pitch_radians), sin(pitch_radians), sin(yaw_radians) * cos(pitch_radians));
			facing.normalize();

			// Then update position

			const float delta_v = e.sprint ? sprint_multiplier * speed * delta_time : speed * delta_time;

			if (e.forward)
				position = position - facing * delta_v;

			if (e.backward)
				position = position + facing * delta_v;


			if (e.left)
				position = position + position.cross(up).normalized() * delta_v;

			if (e.right)
				position = position - position.cross(up).normalized() * delta_v;

			if (e.up)
				position = position + up * delta_v;

			if (e.down)
				position = position - up * delta_v;
		}
	};
}