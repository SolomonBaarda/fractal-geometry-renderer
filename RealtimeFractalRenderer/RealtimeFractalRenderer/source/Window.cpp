#include "Window.h"

Window::Window() : Window(900, 600) {}

Window::~Window()
{
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	delete colours;
}

Window::Window(uint32_t width, uint32_t height) : width(width), height(height), b("Render to window")
{
	colours = new uint8_t[static_cast<int64_t>(width) * static_cast<int64_t>(height) * 4];

	event = SDL_Event();
	events_since_last_get = Events();

	SDL_SetMainReady();

	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow
	(
		"Realtime Fractal Renderer",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		width, height,
		SDL_WINDOW_SHOWN
	);

	renderer = SDL_CreateRenderer
	(
		window,
		-1,
		SDL_RENDERER_ACCELERATED
	);

	SDL_GetRendererInfo(renderer, &info);
	//std::cout << "Renderer name: " << info.name << std::endl;
	//std::cout << "Texture formats: " << std::endl;
	//for (Uint32 i = 0; i < info.num_texture_formats; i++)
	//{
	//	std::cout << SDL_GetPixelFormatName(info.texture_formats[i]) << std::endl;
	//}

	texture = SDL_CreateTexture
	(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		width,
		height
	);

	// Focus mouse on the window
	SDL_WarpMouseInWindow(window, middle_x, middle_y);

	// Hide cursor
	SDL_ShowCursor(SDL_FALSE);

	b.start();
}


/// <summary>
/// 
/// <table>
/// <caption id="multi_row">Complex table</caption>
/// <tr>	<th>Key					<th>Event
/// <tr>	<td>escape				<td>Quit the application
/// <tr>	<td>w					<td>Move camera forwards
/// <tr>	<td>s					<td>Move camera backwards
/// <tr>	<td>a					<td>Move camera left
/// <tr>	<td>d					<td>Move camera right
/// <tr>	<td>q					<td>Move camera up
/// <tr>	<td>e					<td>Move camera down
/// <tr>	<td>lshift				<td>
/// <tr>	<td>g					<td>Take screenshot
/// <tr>	<td>r					<td>debug camera information (position, view direction) to console
/// 
/// </table>
/// 
/// </summary>
/// <returns></returns>
Events Window::get_events()
{
	// Get all events that have occured since last time this function was called 

	Events this_frame = events_since_last_get;
	this_frame.take_screenshot = false;
	this_frame.debug_information = false;

	// Poll window events
	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
			// Exit the application
		case SDL_QUIT:
			exit(0);
			break;

			// Key pressed
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				this_frame.exit = true;
				break;
			case SDLK_w:
				this_frame.forward = true;
				break;
			case SDLK_s:
				this_frame.backward = true;
				break;
			case SDLK_a:
				this_frame.left = true;
				break;
			case SDLK_d:
				this_frame.right = true;
				break;
			case SDLK_q:
				this_frame.up = true;
				break;
			case SDLK_e:
				this_frame.down = true;
				break;
			case SDLK_LSHIFT:
				this_frame.sprint = true;
				break;
			case SDLK_g:
				this_frame.take_screenshot = true;
				break;
			case  SDLK_r:
				this_frame.debug_information = true;
				break;
			default:
				break;
			}
			break;

			// Key released
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				this_frame.exit = false;
				break;
			case SDLK_w:
				this_frame.forward = false;
				break;
			case SDLK_s:
				this_frame.backward = false;
				break;
			case SDLK_a:
				this_frame.left = false;
				break;
			case SDLK_d:
				this_frame.right = false;
				break;
			case SDLK_q:
				this_frame.up = false;
				break;
			case SDLK_e:
				this_frame.down = false;
				break;
			case SDLK_LSHIFT:
				this_frame.sprint = false;
				break;
			case SDLK_g:
				this_frame.take_screenshot = false;
				break;
			case  SDLK_r:
				this_frame.debug_information = false;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}

	// Mouse movement
	SDL_GetMouseState(&this_frame.mouse_pos_x, &this_frame.mouse_pos_y);
	// Calculate mouse delta
	this_frame.delta_mouse_x = this_frame.mouse_pos_x - events_since_last_get.mouse_pos_x;
	this_frame.delta_mouse_y = this_frame.mouse_pos_y - events_since_last_get.mouse_pos_y;
	this_frame.mouse_within_window = this_frame.mouse_pos_x >= 0 && this_frame.mouse_pos_y >= 0 && this_frame.mouse_pos_x < width&& this_frame.mouse_pos_y < height;

	// Move mouse back to the centre of the screen
	this_frame.mouse_pos_x = middle_x;
	this_frame.mouse_pos_y = middle_y;
	SDL_WarpMouseInWindow(window, middle_x, middle_y);

	events_since_last_get = this_frame;
	return this_frame;
}

void Window::set_pixels(uint8_t* pixels)
{
	b.addMarkerNow("start of render");
	SDL_UpdateTexture(texture, NULL, pixels, sizeof(uint8_t) * 4 * width);
	b.addMarkerNow("update texture");

	SDL_RenderClear(renderer);
	b.addMarkerNow("clear render");

	SDL_RenderCopy(renderer, texture, NULL, NULL);
	b.addMarkerNow("copy render");

	SDL_RenderPresent(renderer);
	b.addMarkerNow("present render");

}