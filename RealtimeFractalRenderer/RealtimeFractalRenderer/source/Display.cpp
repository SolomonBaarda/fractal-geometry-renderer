#include "Display.h"

Display::Display() : Display(1920, 1080) {}

Display::~Display()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

Display::Display(int32_t width, int32_t height)
{
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

	const unsigned int texWidth = 1024;
	const unsigned int texHeight = 1024;
	texture = SDL_CreateTexture
	(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		texWidth, texHeight
	);

	std::vector< unsigned char > pixels(texWidth * texHeight * 4, 0);

	bool running = true;
	bool useLocktexture = false;

	unsigned int frames = 0;
	Uint64 start = SDL_GetPerformanceCounter();
}

