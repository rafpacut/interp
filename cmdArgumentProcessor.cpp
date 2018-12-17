#include <filesystem>

namespace cmdArgProcessor
{
	bool isDebugFlag(char* argv)
	{
		return (argv[0] == '-' && argv[1] == 'd');
	}

	bool isCorrectFilePath(char* argv)
	{
		try
		{
			std::filesystem::path pathObj(argv);
			return (std::filesystem::exists(pathObj) &&
				std::filesystem::is_regular_file(pathObj));
		}
		catch(std::filesystem::filesystem_error& e)
		{
			std::cerr << e.what() << std::endl;
		}

		return false;
	}


	std::tuple<char*,bool,bool> process(int argc, char **argv)
	{
		bool runInteractive = true;
		bool debug = false;
		char* filePath;

		//if argc == 1 fire up interactive interpreter -- the default
		if(argc == 2)//either 1) with debug or run non-interactive
		{
			if(isDebugFlag(argv[1]))
				debug = true;	
			else if(isCorrectFilePath(argv[1]))
			{
				runInteractive=false;
				filePath = argv[1];
			}
		}
		if(argc == 3)//debug non-interactive
		{
			if(isDebugFlag(argv[1])) 
				debug = true;
			if(isCorrectFilePath(argv[2]))
			{
				filePath = argv[2];
				runInteractive = false;
			}
		}

		return {filePath, runInteractive, debug};
	}
}
