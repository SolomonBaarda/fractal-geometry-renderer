#pragma once

#include "Events.h"
#include "Vector3.h"

class Camera
{
public:
	Vector3 position, facing, up = Vector3(0, 1, 0);

	float vertical_fov = 40.0f;
	float foucs_distance = 0.1f;

	float speed = 50.0f;

	void update(Events e, float delta_time)
	{
		facing.normalise();
		up.normalise();

		// Update look at direction first


		// Then update position

		float delta_v = speed * delta_time;

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