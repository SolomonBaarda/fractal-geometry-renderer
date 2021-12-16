#pragma once

#include "Events.h"

class Camera
{
public:
	float x = 0.0f, y = 0.0f, z = 0.0f;
	float look_at_x = 0.0f, look_at_y = 0.0f, look_at_z = 0.0f;
	float up_x = 0, up_y = 1, up_z = 0;

	float vertical_fov = 40.0f;
	float foucs_distance = 0.1f;

	float speed = 2;

	void update(Events e, float delta_time)
	{
		// Update look at direction first


		// Then update position

		float delta_v = speed * delta_time;

		if (e.forward)
		{
			x += look_at_x * speed;
			y += look_at_y * speed;
			z += look_at_z * speed;
		}

		if (e.backward)
		{
			x -= look_at_x * speed;
			y -= look_at_y * speed;
			z -= look_at_z * speed;
		}

		if (e.up)
		{
			x += up_x * speed;
			y += up_y * speed;
			z += up_z * speed;
		}

		if (e.down)
		{
			x -= up_x * speed;
			y -= up_y * speed;
			z -= up_z * speed;
		}

	}
};