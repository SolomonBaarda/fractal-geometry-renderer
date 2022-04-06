#include "Renderer.h"

#include <gtest/gtest.h>
#include <string>

namespace
{
	using namespace FractalGeometryRenderer;

	TEST(TestResolution1024, TestDefaultBehaviour)
	{
		Renderer r(1024, 576, std::cout);
	}

	TEST(TestResolution1280, TestDefaultBehaviour)
	{
		Renderer r(1280, 720, std::cout);
	}

	TEST(TestResolution1600, TestDefaultBehaviour)
	{
		Renderer r(1600, 900, std::cout);
	}

	TEST(TestResolution1920, TestDefaultBehaviour)
	{
		Renderer r(1920, 1080, std::cout);
	}

	TEST(TestResolution2560, TestDefaultBehaviour)
	{
		Renderer r(2560, 1440, std::cout);
	}

	TEST(TestResolution3840, TestDefaultBehaviour)
	{
		Renderer r(3840, 2160, std::cout);
	}

	TEST(TestResolution1, TestDefaultBehaviour)
	{
		Renderer r(1, 1, std::cout);
	}

	TEST(TestResolution1234, TestDefaultBehaviour)
	{
		Renderer r(1234, 5678, std::cout);
	}

	TEST(TestResolution_1, TestDefaultBehaviour)
	{
		Renderer r(-1, -2, std::cout);
	}
}
