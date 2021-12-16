#pragma once

struct Events
{
	// Navigation
	bool forward = false, backward = false, up = false, down = false, left = false, right = false;
	float mouse_delta_x = 0.0f, mouse_delta_y = 0.0f;
};