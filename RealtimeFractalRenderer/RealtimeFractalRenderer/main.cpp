#include <cstdint>

#include "FractalGeometryRenderer.hpp"
#include "CLI11.hpp" // Command line parser

int main(int argc, char** argv)
{
	CLI::App app{ "App description" };

	// Add file path as mandatory option
	std::string scene_path;
	app.add_option("-s,--scene", scene_path, "Path to the file containing the scene source code, written in the OpenCL C kernel language")->required();

	std::vector<std::string> include_directories;
	// Add build options
	app.add_option("-i,--include", include_directories, "List of paths of directories containing C header files or OpenCL C kernel files to be included in the scene");

	std::pair<uint32_t, uint32_t> resolution{ 0, 0 };
	// Add desired resolution
	app.add_option("-r,--resolution", resolution, "");


	CLI11_PARSE(app, argc, argv);

	FractalGeometryRenderer* r;

	// Resolution was not specified
	if (resolution.first == 0u || resolution.second == 0u)
	{
		r = new FractalGeometryRenderer();
	}
	// Resolution was specified
	else
	{
		r = new FractalGeometryRenderer(resolution.first, resolution.second);
	}


	// No additional include directories were specified
	if (include_directories.empty())
	{
		r->run(scene_path);
	}
	// Some were
	else
	{
		// Combine all include paths into one string of build options
		std::string build_options = "";
		for (std::string s : include_directories)
		{
			build_options += "-I \"" + s + "\" ";
		}

		r->run(scene_path, build_options);
	}

	return 0;
}
