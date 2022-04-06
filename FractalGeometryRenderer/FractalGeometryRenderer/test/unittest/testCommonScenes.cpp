#include "Renderer.h"

#include <gtest/gtest.h>
#include <string>

// Variable must be set in CMake
#ifndef TEST_PATH_TO_KERNELS
#error "TEST_PATH_TO_KERNELS must be set using CMake"
#endif

// Cmake usually copies the scene files to "<binary dir>/FractalGeometryRenderer" when CMake cache is refreshed

namespace
{
	using namespace FractalGeometryRenderer;

	const std::string path = TEST_PATH_TO_KERNELS;
	const std::string build_options = "-I " + path + " -I " + path + "include";
	const size_t size = 1;

	TEST(TestLoadSceneHelloWorld, TestDefaultBehaviour)
	{
		Renderer r(1920, 1080, std::cout);
		r.load_scene(path + "hello_world.cl", build_options, size);
	}

	TEST(TestLoadSceneInfiniteSpheres, TestDefaultBehaviour)
	{
		Renderer r(1920, 1080, std::cout);
		r.load_scene(path + "infinite_spheres.cl", build_options, size);
	}

	TEST(TestLoadSceneMandelbrot, TestDefaultBehaviour)
	{
		Renderer r(1920, 1080, std::cout);
		r.load_scene(path + "mandelbrot.cl", build_options, size);
	}

	TEST(TestLoadSceneMandelbrotZoom, TestDefaultBehaviour)
	{
		Renderer r(1920, 1080, std::cout);
		r.load_scene(path + "mandelbrot_zoom.cl", build_options, size);
	}

	TEST(TestLoadSceneMandelbulb, TestDefaultBehaviour)
	{
		Renderer r(1920, 1080, std::cout);
		r.load_scene(path + "mandelbulb.cl", build_options, size);
	}

	TEST(TestLoadSceneMandelbulbCrossSection, TestDefaultBehaviour)
	{
		Renderer r(1920, 1080, std::cout);
		r.load_scene(path + "mandelbulb_cross_section.cl", build_options, size);
	}

	TEST(TestLoadScenePlanet, TestDefaultBehaviour)
	{
		Renderer r(1920, 1080, std::cout);
		r.load_scene(path + "planet.cl", build_options, size);
	}

	TEST(TestLoadSceneSierpinskiCollection, TestDefaultBehaviour)
	{
		Renderer r(1920, 1080, std::cout);
		r.load_scene(path + "sierpinski_collection.cl", build_options, size);
	}

	TEST(TestLoadSceneSierpinskiCube, TestDefaultBehaviour)
	{
		Renderer r(1920, 1080, std::cout);
		r.load_scene(path + "sierpinski_cube.cl", build_options, size);
	}

	TEST(TestLoadSceneSierpinskiTetrahedron, TestDefaultBehaviour)
	{
		Renderer r(1920, 1080, std::cout);
		r.load_scene(path + "sierpinski_tetrahedron.cl", build_options, size);
	}

	TEST(TestLoadSceneSphereBox, TestDefaultBehaviour)
	{
		Renderer r(1920, 1080, std::cout);
		r.load_scene(path + "sphere_box.cl", build_options, size);
	}

	TEST(TestLoadSceneTerrain, TestDefaultBehaviour)
	{
		Renderer r(1920, 1080, std::cout);
		r.load_scene(path + "terrain.cl", build_options, size);
	}

	TEST(TestLoadSceneTrivial, TestDefaultBehaviour)
	{
		Renderer r(1920, 1080, std::cout);
		r.load_scene(path + "trivial.cl", build_options, size);
	}

}
