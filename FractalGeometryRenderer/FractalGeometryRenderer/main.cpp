#include <cstdint>

#include "FractalGeometryRenderer.hpp"
#include "CLI11.hpp" // Command line parser

#include <filesystem>
#include <iostream>
#include <fstream>

int main(int argc, char** argv)
{
	CLI::App app{ "App description" };

	// Add file path as mandatory option
	std::string scene_path;
	app.add_option("-s,--scene", scene_path, "String path to the file containing the scene source code, written in the OpenCL C kernel language")->required();

	std::vector<std::string> additional_include_directories;
	// Add build options
	app.add_option("-i,--include", additional_include_directories, "Space seperated string list of additional directory paths containing C header files or OpenCL C kernel files, which should be included in the scene");

	std::pair<uint32_t, uint32_t> resolution{ 0, 0 };
	// Add desired resolution
	app.add_option("-r,--resolution", resolution, "Space seperated resolution width and height in pixels. The value of width x height must be a multiple of two");

	size_t desired_work_group_size = 0;
	app.add_option("-w,--work-group-size", desired_work_group_size, "Desired group size when distrubuting work over the GPU. Must be a multiple of resolution width x height");

	// Parse the command line arguments
	CLI11_PARSE(app, argc, argv);

	

	// Resolution was not specified
	if (resolution.first == 0u || resolution.second == 0u)
	{
		resolution.first = 1920;
		resolution.second = 1080;

		printf("Invalid resolution specified, using default value of %u x %u", resolution.first, resolution.second);
	}

	FractalGeometryRenderer::FractalGeometryRenderer r(resolution.first, resolution.second);


	// Add the default include directory path
	additional_include_directories.push_back("kernels/include");

	// Combine all include paths into one string of build options
	std::string build_options = "";
	for (std::string s : additional_include_directories)
	{
		std::filesystem::path path(s);

		if (std::filesystem::is_directory(path))
		{
			build_options += "-I \"" + path.string() + "\" ";

			for (const auto& entry : std::filesystem::recursive_directory_iterator(s))
			{
				if (entry.is_directory())
				{
					build_options += "-I \"" + entry.path().string() + "\" ";
				}
			}
		}
	}

	std::filebuf fb;
	fb.open("results.txt", std::ios::out);
	std::ostream data(&fb);

	//printf("%s\n\n", build_options.c_str());
	r.run(scene_path, build_options, desired_work_group_size, data);

	fb.close();

	return 0;
}
