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


	//std::filebuf buffer_log;
	//buffer_log.open("log.txt", std::ios::app);
	//std::ostream stream_log(&buffer_log);
	std::ostream& stream_log = std::cout;

	stream_log << "--------------------------------------------------------------------------------\n";


	// Parse the command line arguments
	CLI11_PARSE(app, argc, argv);



	// Resolution was not specified
	if (resolution.first == 0u || resolution.second == 0u)
	{
		resolution.first = 1920;
		resolution.second = 1080;

		printf("\nInvalid resolution specified, using default value of %u x %u", resolution.first, resolution.second);
	}



	std::filebuf buffer_results;
	buffer_results.open("results.txt", std::ios::app);
	std::ostream stream_data(&buffer_results);


	FractalGeometryRenderer::FractalGeometryRenderer r(resolution.first, resolution.second, stream_log);


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

	//printf("%s\n\n", build_options.c_str());
	r.run(scene_path, build_options, desired_work_group_size, stream_data);

	std::stringstream ss;
	ss << stream_log.rdbuf();
	std::string myString = ss.str();
	printf("%s", myString.c_str());

	//buffer_log.close();
	buffer_results.close();

	return 0;
}
