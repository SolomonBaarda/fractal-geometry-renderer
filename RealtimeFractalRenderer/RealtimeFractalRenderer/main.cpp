#include "FractalGeometryRenderer.hpp"
#include "CLI11.hpp"

int main(int argc, char** argv)
{
	CLI::App app{ "App description" };

	// Add file path as mandatory option
	std::string scene_path;
	app.add_option("-s,--scene", scene_path, "Path to the file containing the scene source code, written in the OpenCL C kernel language")->required();

	std::vector<std::string> include_directories;
	// Add build 
	app.add_option("-i,--include", include_directories, "List of paths of directories containing C header files or OpenCL C kernel files to be included in the scene");

	CLI11_PARSE(app, argc, argv);

	FractalGeometryRenderer r;

	if (include_directories.empty())
	{
		r.run(scene_path);
	}
	else
	{
		// Combine all include paths into one string of build options
		std::string build_options = "";
		for (std::string s : include_directories)
		{
			build_options += "-I \"" + s + "\" ";
		}

		r.run(scene_path, build_options);
	}



	return 0;
}
