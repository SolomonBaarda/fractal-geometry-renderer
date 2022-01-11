#pragma once

#include <cstdint>

struct Events
{
	// Keyboard buttons
	bool forward = false, backward = false, up = false, down = false, left = false, right = false;
	bool sprint = false;

	// Mouse
	int32_t mouse_pos_x = 0, mouse_pos_y = 0, delta_mouse_x = 0, delta_mouse_y = 0;
	bool mouse_within_window = false;
};