#pragma once

#define SDL_MAIN_HANDLED

#include "Vector3.h"
#include "Events.h"
#include "Benchmark.h"

#include <SDL.h>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cstring>

namespace FractalGeometryRenderer
{
	/// <summary>
	/// Class used for interacting with the display window window. 
	/// </summary>
	class Window
	{
	public:
		Window(uint32_t width, uint32_t height);
		~Window();

		/// <summary>
		/// <table>
		/// <caption id="multi_row">Keyboard events</caption>
		/// <tr>	<th>Key			<th>Event
		/// <tr>	<td>escape		<td>Quit the application
		/// <tr>	<td>w			<td>Move camera forwards
		/// <tr>	<td>s			<td>Move camera backwards
		/// <tr>	<td>a			<td>Move camera left
		/// <tr>	<td>d			<td>Move camera right
		/// <tr>	<td>q			<td>Move camera up
		/// <tr>	<td>e			<td>Move camera down
		/// <tr>	<td>lshift		<td>
		/// <tr>	<td>g			<td>Take screenshot
		/// <tr>	<td>r			<td>debug camera information (position, view direction) to console
		/// </table>
		/// </summary>
		/// <returns>Any window events that occured since the last time this method was called</returns>
		Events get_events();

		/// <summary>
		/// Sets the pixels to be displayed for the current display window. Pixels are in the form RGBA 
		/// 0-255 and the array has a capacity of width x height.
		/// </summary>
		/// <param name="pixels"></param>
		void set_pixels(uint8_t* pixels);

	private:
		const uint32_t width, height;
		const int32_t middle_x = width / 2, middle_y = height / 2;

		uint8_t* colours;

		SDL_Window* window;
		SDL_Renderer* renderer;
		SDL_RendererInfo info;
		SDL_Texture* texture;
		SDL_Event event;

		Events events_since_last_get;
		SDL_bool capture_mouse = SDL_FALSE;

		Profiling::Benchmark b;

		inline float clamp01(float x)
		{
			return x < 0 ? 0 : x > 1 ? 1 : x;
		}

		inline uint8_t toInt(float x)
		{
			return static_cast<uint8_t>(clamp01(x) * 255);
			// Applies a gamma correction of 2.2
			return static_cast<uint8_t>(pow(clamp01(x), 1 / 2.2) * 255 + .5);
		}
	};
}