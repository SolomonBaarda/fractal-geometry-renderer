#include "Renderer.h"

void Renderer::render()
{
#pragma omp parallel for schedule(dynamic, 1)  // OpenMP

	// Rows
	for (int32_t y = 0; y < height; y++)
	{
		fprintf(stderr, "\rRendering %5.2f%%", 100.0f * static_cast<float>(y) / static_cast<float>(height - 1));

		// Columns
		for (int32_t x = 0; x < width; x++)
		{
			float u = static_cast<float>(x) / (width - 1);
			float v = static_cast<float>(y) / (height - 1);

			Ray r = camera.getCameraRay(u, v);
			buffer[y * width + x] = calculatePixelColour(r.origin, r.direction);
		}
	}
}