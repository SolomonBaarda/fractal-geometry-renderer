#include "Window.h"

namespace FractalGeometryRenderer
{
	Window::~Window()
	{
		SDL_DestroyTexture(texture);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	Window::Window(uint32_t width, uint32_t height, std::ostream& log) :
		width(width), height(height), aspect_ratio(static_cast<float>(width) / static_cast<float>(height)),
		log(log), event(), events_since_last_get(), pixels_pitch(sizeof(uint8_t) * 4 * width), 
		pixels_size(sizeof(uint8_t) * 4 * width * height), b("Render to window", log)
	{
		pixels = new uint8_t[static_cast<int64_t>(width) * static_cast<int64_t>(height) * 4];

		SDL_SetMainReady();

		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

		window = SDL_CreateWindow
		(
			"Fractal Geometry Renderer",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			width, height,
			SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
		);

		renderer = SDL_CreateRenderer
		(
			window,
			-1,
			SDL_RENDERER_ACCELERATED
		);


		// Debug the renderer name
		SDL_RendererInfo info;
		SDL_GetRendererInfo(renderer, &info);
		log << "Chosen renderer: " << info.name << "\n";
		log << "Running at resolution : " << width << "x" << height << "\n";
		log << ("\n");

		log << "Supported texture formats:\n";
		for (int32_t i = 0; i < info.num_texture_formats; i++)
		{
			log << "\t" << SDL_GetPixelFormatName(info.texture_formats[i]) << "\n";
		}
		log << ("\n");

		texture = SDL_CreateTexture
		(
			renderer,
			SDL_PIXELFORMAT_ABGR8888, // RGBA ordering
			SDL_TEXTUREACCESS_STREAMING,
			width,
			height
		);

		// Set the logical size of the window (forces correct aspect ratio after resizing)
		SDL_RenderSetLogicalSize(renderer, width, height);

		b.start();
	}

	Events Window::get_events()
	{
		// Get all events that have occured since last time this function was called 

		Events this_frame = events_since_last_get;
		this_frame.take_screenshot = false;
		this_frame.debug_information = false;

		bool ignoreMouseEventsThisFrame = false;

		// Poll window events
		while (SDL_PollEvent(&event) != 0)
		{
			switch (event.type)
			{
				// Exit the application
			case SDL_QUIT:
				this_frame.exit = true;
				break;

				// Capture the mouse if clicked on the screen
			case SDL_MOUSEBUTTONDOWN:
				capture_mouse = true;
				ignoreMouseEventsThisFrame = true;
				break;

				// Key pressed
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					capture_mouse = false;
					ignoreMouseEventsThisFrame = true;
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

		if (capture_mouse)
		{
			// Mouse movement
			SDL_GetMouseState(&this_frame.mouse_pos_x, &this_frame.mouse_pos_y);
			// Calculate mouse delta
			this_frame.mouse_screen_delta_x = static_cast<float>(this_frame.mouse_pos_x - events_since_last_get.mouse_pos_x) / static_cast<float>(width);
			this_frame.mouse_screen_delta_y = static_cast<float>(this_frame.mouse_pos_y - events_since_last_get.mouse_pos_y) / static_cast<float>(height);
			this_frame.mouse_within_window = this_frame.mouse_pos_x >= 0 && this_frame.mouse_pos_y >= 0 && this_frame.mouse_pos_x < width&& this_frame.mouse_pos_y < height;

			// Move mouse back to the centre of the screen
			this_frame.mouse_pos_x = middle_x;
			this_frame.mouse_pos_y = middle_y;

			// Hide cursor
			SDL_ShowCursor(SDL_FALSE);

			// Move it to the middle of the screen
			SDL_WarpMouseInWindow(window, middle_x, middle_y);
		}
		else
		{
			// Enable cursor
			SDL_ShowCursor(SDL_TRUE);
		}

		if (ignoreMouseEventsThisFrame)
		{
			this_frame.mouse_screen_delta_x = 0;
			this_frame.mouse_screen_delta_y = 0;
		}

		events_since_last_get = this_frame;
		return this_frame;
	}

	void Window::set_pixels(uint8_t* new_pixels)
	{
		b.addMarkerNow("start of render");

		SDL_RenderClear(renderer);
		b.addMarkerNow("clear render");

		SDL_LockTexture(texture, NULL, (void**)&pixels, &pixels_pitch);
		b.addMarkerNow("lock texture");

		//SDL_UpdateTexture(texture, NULL, pixels, texture_pitch);
		//b.addMarkerNow("update texture");

		SDL_memcpy(pixels, new_pixels, pixels_size);
		b.addMarkerNow("copy pixels");

		SDL_UnlockTexture(texture);
		b.addMarkerNow("unlock texture");

		//SDL_RenderSetScale(renderer, 0.5, 0.5f);

		SDL_RenderCopy(renderer, texture, NULL, NULL);
		b.addMarkerNow("copy render");

		SDL_RenderPresent(renderer);
		b.addMarkerNow("present render");
	}
}