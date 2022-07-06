#include <benchmark/benchmark.h>
#include <SAMOSBOR/SAMOSBOR.h>

namespace fs = std::filesystem;
namespace core = SAMOSBOR::core;
namespace assembly = SAMOSBOR::assembly::ref;

using AssemblySettings = assembly::AssemblySettings;
using AssemblyGraphSettings = assembly::AssemblyGraphSettings;
using AssemblyBuilder = assembly::AssemblyBuilder;

using Assembly_Builder = SAMOSBOR::assembly::Assembly_Builder;

static void DoSetup(const benchmark::State& state) 
{
	const fs::path outputDirPath = fs::path("../../../data/output/");

	if (!fs::exists(outputDirPath))
	{
		fs::create_directory(outputDirPath);
	}
}

static void DoTeardown(const benchmark::State& state) 
{
	const fs::path outputDirPath = fs::path("../../../data/output/");

	for (auto& path : fs::directory_iterator(outputDirPath))
	{
		fs::remove_all(path);
	}
}

template <class ...Args>
void AssemblyCreationGraphId(benchmark::State& state, Args&&... args)
{
	auto args_tuple = std::make_tuple(std::move(args)...);

	std::string graphId = std::get<0>(args_tuple);
	fs::path dataPath = std::get<1>(args_tuple);
	fs::path outputPath = std::get<2>(args_tuple);

	AssemblySettings settings
	{
		.dataPath = dataPath,
		.outputPath = outputPath,
		.graphSettings = AssemblyGraphSettings
		{
			.extensionLength = 0
		}
	};

	AssemblyBuilder builder;

	for (auto _ : state)
	{
		benchmark::DoNotOptimize(builder.Build(graphId, settings));
	}
}

template <class ...Args>
void AssemblyCreationJsonBoost(benchmark::State& state, Args&&... args)
{
	auto args_tuple = std::make_tuple(std::move(args)...);

	fs::path structureFilePath = std::get<0>(args_tuple);
	fs::path outputFilePath = std::get<1>(args_tuple);

	std::string structureFile = structureFilePath.string();
	std::string outputFile = outputFilePath.string();

	Assembly_Builder builder;
	
	for (auto _ : state)
	{
		builder.Build(structureFile, outputFile, 0.0f);
	}
}

BENCHMARK_CAPTURE(
	AssemblyCreationGraphId,
	AssemblyTwoNodesGraphId, 
	std::string("15107_0h.20_1t#0+1"), 
	fs::path("../../../data/tool_3"), 
	fs::path("../../../data/output/assembly_3.stp"))
		->Setup(DoSetup)
		->Teardown(DoTeardown)
		->Repetitions(10)
		->DisplayAggregatesOnly()
		->Unit(benchmark::kMillisecond)
		->MeasureProcessCPUTime();

BENCHMARK_CAPTURE(
	AssemblyCreationJsonBoost,
	AssemblyTwoNodesJson,
	fs::path("../../../data/tool_3/assembly.json"),
	fs::path("../../../data/output/assembly_3.stp"))
		->Setup(DoSetup)
		->Teardown(DoTeardown)
		->Repetitions(10)
		->DisplayAggregatesOnly()
		->Unit(benchmark::kMillisecond)
		->MeasureProcessCPUTime();