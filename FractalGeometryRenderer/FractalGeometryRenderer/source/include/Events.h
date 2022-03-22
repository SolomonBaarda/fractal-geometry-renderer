#pragma once

#include <cstdint>

namespace FractalGeometryRenderer
{
	/// <summary>
	/// Class used for storing keyboard and mouse events that have occured.
	/// </summary>
	struct Events
	{
		// Keyboard buttons
		bool forward = false, backward = false, up = false, down = false, left = false, right = false;
		bool sprint = false, take_screenshot = false, debug_information = false, exit = false;

		// Mouse
		int32_t mouse_pos_x = 0, mouse_pos_y = 0;

		float mouse_screen_delta_x = 0, mouse_screen_delta_y = 0;
		bool mouse_within_window = false;
	};
}