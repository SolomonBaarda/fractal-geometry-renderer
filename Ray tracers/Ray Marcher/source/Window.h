#pragma once
#include <vector>




class Window
{
private:
	SDL_Event event;
	SDL_Renderer* renderer;
	SDL_Window* window;
	SDL_Texture* texture;
	std::vector< unsigned char > pixels;

	int32_t width, height;

public:
	Window(int32_t width, int32_t height) : width(width), height(height)
	{
		SDL_Init(SDL_INIT_VIDEO);

		window = SDL_CreateWindow("window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);

		pixels = std::vector< unsigned char >(width * height * 4, 0);

		SDL_Event event;
		bool running = true;
		bool useLocktexture = false;
	}

	~Window()
	{
		// Clean up

		SDL_DestroyTexture(texture);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	void setPixels(Vector3* buffer)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		for (int i = 0; i < width * height; i++)
		{
			const int32_t index = i * 4;
			pixels[index + 0] = buffer[i].z;        // b
			pixels[index + 1] = buffer[i].y;        // g
			pixels[index + 2] = buffer[i].x;        // r
			pixels[index + 3] = SDL_ALPHA_OPAQUE;    // a
		}

		unsigned char* lockedPixels = nullptr;
		int pitch = 0;
		SDL_LockTexture(texture, NULL, reinterpret_cast<void**>(&lockedPixels), &pitch);
		std::memcpy(lockedPixels, pixels.data(), pixels.size());
		SDL_UnlockTexture(texture);

		//SDL_UpdateTexture(texture, NULL, pixels.data(), width * 4);

		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

};