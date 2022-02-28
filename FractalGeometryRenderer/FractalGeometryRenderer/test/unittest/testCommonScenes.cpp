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
	const std::string build_options = "-I " + path + "include";

	TEST(TestLoadSceneMandelbulb, TestDefaultBehaviour)
	{
		std::ostream& s = std::cout;
		Renderer r(1920, 1080, s);

		r.load_scene(path + "mandelbulb.cl", build_options, 256);
	}

	TEST(TestLoadSceneSierpinskiCube, TestDefaultBehaviour)
	{
		std::ostream& s = std::cout;
		Renderer r(1920, 1080, s);

		r.load_scene(path + "sierpinski_cube.cl", build_options, 256);
	}

	TEST(TestLoadSceneSierpinskiTetrahedron, TestDefaultBehaviour)
	{
		std::ostream& s = std::cout;
		Renderer r(1920, 1080, s);

		r.load_scene(path + "sierpinski_tetrahedron.cl", build_options, 256);
	}

	TEST(TestLoadSceneSphereBox, TestDefaultBehaviour)
	{
		std::ostream& s = std::cout;
		Renderer r(1920, 1080, s);

		r.load_scene(path + "sphere_box.cl", build_options, 256);
	}

	TEST(TestLoadSceneInfiniteSpheres, TestDefaultBehaviour)
	{
		std::ostream& s = std::cout;
		Renderer r(1920, 1080, s);

		r.load_scene(path + "infinite_spheres.cl", build_options, 256);
	}
}
