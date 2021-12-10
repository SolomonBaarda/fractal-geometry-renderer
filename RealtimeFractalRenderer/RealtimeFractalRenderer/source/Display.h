#pragma once

#define SDL_MAIN_HANDLED

#include <cstdint>
#include <SDL.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cstring>

class Display
{
public:
	Display();
	Display(int32_t width, int32_t height);
	~Display();

	//void set_pixels();

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_RendererInfo info;
	SDL_Texture* texture;
	SDL_Event event;
};





//int main(int argc, char** argv)
//{
//
//
//	while (running)
//	{
//
//		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
//		SDL_RenderClear(renderer);
//
//		while (SDL_PollEvent(&event))
//		{
//			if ((SDL_QUIT == event.type) ||
//				(SDL_KEYDOWN == event.type && SDL_SCANCODE_ESCAPE == event.key.keysym.scancode))
//			{
//				running = false;
//				break;
//			}
//			if (SDL_KEYDOWN == event.type && SDL_SCANCODE_L == event.key.keysym.scancode)
//			{
//				useLocktexture = !useLocktexture;
//				std::cout << "Using " << (useLocktexture ? "SDL_LockTexture() + memcpy()" : "SDL_UpdateTexture()") << std::endl;
//			}
//		}
//
//		// splat down some random pixels
//		for (unsigned int i = 0; i < 1000; i++)
//		{
//			const unsigned int x = rand() % texWidth;
//			const unsigned int y = rand() % texHeight;
//
//			const unsigned int offset = (texWidth * 4 * y) + x * 4;
//			pixels[offset + 0] = rand() % 256;        // b
//			pixels[offset + 1] = rand() % 256;        // g
//			pixels[offset + 2] = rand() % 256;        // r
//			pixels[offset + 3] = SDL_ALPHA_OPAQUE;    // a
//		}
//
//		if (useLocktexture)
//		{
//			unsigned char* lockedPixels = nullptr;
//			int pitch = 0;
//			SDL_LockTexture
//			(
//				texture,
//				NULL,
//				reinterpret_cast<void**>(&lockedPixels),
//				&pitch
//			);
//			std::memcpy(lockedPixels, pixels.data(), pixels.size());
//			SDL_UnlockTexture(texture);
//		}
//		else
//		{
//			SDL_UpdateTexture
//			(
//				texture,
//				NULL,
//				pixels.data(),
//				texWidth * 4
//			);
//		}
//
//		SDL_RenderCopy(renderer, texture, NULL, NULL);
//		SDL_RenderPresent(renderer);
//
//		frames++;
//		const Uint64 end = SDL_GetPerformanceCounter();
//		const static Uint64 freq = SDL_GetPerformanceFrequency();
//		const double seconds = (end - start) / static_cast<double>(freq);
//		if (seconds > 2.0)
//		{
//			std::cout
//				<< frames << " frames in "
//				<< std::setprecision(1) << std::fixed << seconds << " seconds = "
//				<< std::setprecision(1) << std::fixed << frames / seconds << " FPS ("
//				<< std::setprecision(3) << std::fixed << (seconds * 1000.0) / frames << " ms/frame)"
//				<< std::endl;
//			start = end;
//			frames = 0;
//		}
//	}
//
//
//
//	return 0;
//}


