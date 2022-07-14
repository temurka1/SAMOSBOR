#include <benchmark/benchmark.h>
#include <SAMOSBOR/SAMOSBOR.h>

namespace fs = std::filesystem;
namespace core = SAMOSBOR::core;
namespace assembly = SAMOSBOR::assembly::ref;

using AssemblySettings = assembly::AssemblySettings;
using AssemblyGraphSettings = assembly::AssemblyGraphSettings;
using AssemblyBuilder = assembly::AssemblyBuilder;

#define USE_ABSOLUTE

#define ABSOLUTE_DATA_PATH "D:/temurka1/SAMOSBOR/source/x64/Debug/data/tool_5"
#define RELATIVE_DATA_PATH "data/tool_5"

#ifdef USE_ABSOLUTE
#define DATA_PATH ABSOLUTE_DATA_PATH
#else
#define DATA_PATH RELATIVE_DATA_PATH
#endif

static void DoSetup(const benchmark::State& state) 
{
	const fs::path outputDirPath = fs::path("output/");

	if (!fs::exists(outputDirPath))
	{
		fs::create_directory(outputDirPath);
	}
}

static void DoTeardown(const benchmark::State& state) 
{
	const fs::path outputDirPath = fs::path("output/");

	if (fs::exists(outputDirPath))
	{
		for (auto& path : fs::directory_iterator(outputDirPath))
		{
			fs::remove_all(path);
		}
	}
}

template <class ...Args>
void AssemblyCreation_Ref(benchmark::State& state, Args&&... args)
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

BENCHMARK_CAPTURE(
	AssemblyCreation_Ref,
	TwoNodes, 
	std::string("avtA_0.Qlwi_1.LoOYVa_2.wX_3.ZfTr_4.wNC_5#0+1.1+2@3235633.1+3@3235634.1+4@49.2+5@3235633"), 
	fs::path(DATA_PATH),
	fs::path("output/assembly_5.stp"))
		->Setup(DoSetup)
		->Teardown(DoTeardown)
		->Iterations(1)
		->DisplayAggregatesOnly()
		->MeasureProcessCPUTime()
		->Unit(benchmark::kMillisecond);