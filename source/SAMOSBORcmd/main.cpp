#include "pch.h"

namespace fs = std::filesystem;

using ArgumentParser = argparse::ArgumentParser;
using AssemblyBuilder = SAMOSBOR::assembly::ref::AssemblyBuilder;
using AssemblySettings = SAMOSBOR::assembly::ref::AssemblySettings;
using AssemblyGraphSettings = SAMOSBOR::assembly::ref::AssemblyGraphSettings;

using Result = SAMOSBOR::core::Result;

// Usage: 
// SAMOSBORcmd.exe --id 15107_0h.20_1t#0+1 --i data/tool3 --o assembly.stp --l 40

int main(int argc, char* argv[])
{
	ArgumentParser program("SAMOSBORcmd");

	program.add_description("Command-line tool to create tool assembly STEP model");

	program.add_argument("--id").help("assembly graph id string").required();
	program.add_argument("--i").help("path with input STEP files").required();
	program.add_argument("--o").help("output tool assembly file").required();
	program.add_argument("--l").help("extension length").default_value(0).scan<'i', int>();
	program.add_argument("help");

	program.parse_args(argc, argv);

	AssemblySettings settings
	{
		.dataPath = fs::current_path() / program.get<std::string>("--i"),
		.outputPath = fs::current_path() / program.get<std::string>("--o"),
		.graphSettings = AssemblyGraphSettings
		{
			.extensionLength = program.get<float>("--l")
		}
	};

	AssemblyBuilder assemblyBuilder;
	Result result = assemblyBuilder.Build(program.get<std::string>("--s"), settings);

	if (!result.Ok())
	{
		std::cout << result.Error() << std::endl;
		return -1;
	}

	return 0;
}
