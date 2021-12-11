#include "Renderer.h"
#include "constants.h"

Renderer::Renderer() : Renderer(900, 600) { }

Renderer::Renderer(uint32_t width, uint32_t height) : width(width), height(height)
{
	float aspect_ratio = static_cast<float>(width) / static_cast<float>(height);

	camera = Camera(Vector3(-10, -5, -10), Vector3(0, 0, 0), Vector3(0, 1, 0), 40.0f, aspect_ratio, 0.1f);
	buffer = new Vector3[static_cast<int64_t>(width) * static_cast<int64_t>(height)];
}

Renderer::~Renderer()
{

}

void Renderer::render()
{
#pragma omp parallel for schedule(dynamic, 1)  // OpenMP

	// Rows
	for (int32_t y = 0; y < height; y++)
	{
		//fprintf(stderr, "\rRendering %5.2f%%", 100.0f * static_cast<float>(y) / static_cast<float>(height - 1));

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