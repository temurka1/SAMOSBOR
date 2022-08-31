#include "pch.h"

namespace fs = std::filesystem;

using ArgumentParser = argparse::ArgumentParser;

using AssemblySettings = SAMOSBOR::assembly::ref::AssemblySettings;

using Pipeliner = SAMOSBOR::workflow::Pipeliner;
using PipelineOptions = SAMOSBOR::workflow::PipelineOptions;

using Result = SAMOSBOR::core::Result;

// Usage: 
// SAMOSBORcmd.exe --id 15107_0h.20_1t#0+1 --i data/tool3 --o assembly_out --l 40 --gltf --step

int main(int argc, char* argv[])
{
	ArgumentParser program("SAMOSBORcmd");

	program.add_description("Command-line tool to create tool assembly STEP model");

	program.add_argument("--id").help("assembly graph id").required();
	program.add_argument("--i").help("input path").required();
	program.add_argument("--o").help("output path").required();
	program.add_argument("--l").help("extension length").default_value(0).scan<'i', int>();
	program.add_argument("--trc").help("triangulation coefficient").default_value(0.33).scan<'g', double>();
	program.add_argument("--gltf").help("output tool assembly as glTF").default_value(false).implicit_value(true);
	program.add_argument("--step").help("output tool assembly as STEP").default_value(false).implicit_value(true);
	program.add_argument("help");

	program.parse_args(argc, argv);

	AssemblySettings settings
	{
		.graphId = program.get<std::string>("--id"),
		.dataPath = fs::current_path() / program.get<std::string>("--i"),
		.outputPath = fs::current_path() / program.get<std::string>("--o"),
		.extensionLength = program.get<float>("--l"),
		.triangulationCoefficient = program.get<double>("--trc")
	};

	uint32_t pipelineOptions = 0;

	if (program.get<bool>("--step"))
	{
		pipelineOptions |= PipelineOptions::EXPORT_STEP;
	}

	if (program.get<bool>("--gltf"))
	{
		pipelineOptions |= PipelineOptions::EXPORT_GLTF;
	}

	Pipeliner pipeliner;
	Result result = pipeliner.RunExportPipeline(settings, pipelineOptions);

	if (!result.Ok())
	{
		std::cout << result.Error() << std::endl;
		return -1;
	}

	return 0;
}
