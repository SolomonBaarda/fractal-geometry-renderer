#include "Display.h"
#include "constants.h"

Display::Display() : Display(900, 600) {}

Display::~Display()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(texture);
	SDL_Quit();
}

Display::Display(uint32_t width, uint32_t height) : width(width), height(height)
{
	colours = new uint8_t[static_cast<int64_t>(width) * static_cast<int64_t>(height)];

	SDL_SetMainReady();

	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow
	(
		"SDL2",
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
	std::cout << "Renderer name: " << info.name << std::endl;
	std::cout << "Texture formats: " << std::endl;
	for (Uint32 i = 0; i < info.num_texture_formats; i++)
	{
		std::cout << SDL_GetPixelFormatName(info.texture_formats[i]) << std::endl;
	}

	texture = SDL_CreateTexture
	(
		renderer,
		SDL_PIXELFORMAT_RGB888,
		SDL_TEXTUREACCESS_STREAMING,
		width,
		height
	);
}

void Display::poll_events()
{
	while (SDL_PollEvent(&event) != 0)
	{
		if (event.type == SDL_QUIT)
		{
			exit(0);
		}
	}
}

void Display::set_pixels(Vector3* new_colours)
{
	for (uint32_t y = 0; y < height; y++)
	{
		for (uint32_t x = 0; x < width; x++)
		{
			int index = y * width + x;

			colours[index] = toInt(new_colours[index].x);
			colours[index + 1] = toInt(new_colours[index].y);
			colours[index + 2] = toInt(new_colours[index].z);
		}
	}

	SDL_UpdateTexture
	(
		texture,
		NULL,
		colours,
		sizeof(uint8_t) * 3
	);

	int e = SDL_RenderClear(renderer);
	if (e < 0)
	{
		std::cout << "ERROR:" << SDL_GetError();
	}

	e = SDL_RenderCopy(renderer, texture, NULL, NULL);
	if (e < 0)
	{
		std::cout << "ERROR:" << SDL_GetError();
	}

	SDL_RenderPresent(renderer);
}