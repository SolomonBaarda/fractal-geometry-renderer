#include "Window.h"

Window::Window() : Window(900, 600) {}

Window::~Window()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(texture);
	SDL_Quit();
}

Window::Window(uint32_t width, uint32_t height) : width(width), height(height) {
	colours = new uint8_t[static_cast<int64_t>(width) * static_cast<int64_t>(height) * 4];

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
}

Events Window::get_events()
{
	// get all events that have occured since last time this function was called 

	Events this_frame;

	while (SDL_PollEvent(&event) != 0)
	{
		if (event.type == SDL_QUIT)
		{
			exit(0);
		}

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
			default:
				break;
			}
			break;

			// Key released
		case SDL_KEYUP:
			switch (event.key.keysym.sym) {
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
				default:
					break;
				}
				break;
			}
			break;

		default:
			break;
		}
	}

	events_since_last_get = &this_frame;
	return *events_since_last_get;
}



void Window::set_pixels(uint8_t* pixels)
{
	SDL_UpdateTexture(texture, NULL, pixels, sizeof(uint8_t) * 4 * width);

	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}