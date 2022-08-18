#include "pch.h"
#include "Pipeliner.h"
#include "Result.hpp"
#include "Assembly.h"
#include "AssemblyGraph.h"
#include "AssemblySettings.hpp"
#include "GraphId.h"
#include "EmptyTypeT.hpp"
#include "StepData.h"

#include "ReadStepStage.h"
#include "BuildAssemblyGraphStage.h"
#include "BuildAssemblyStage.h"
#include "WriteStepStage.h"
#include "WriteGltfStage.h"

#include <pipeline/pipeline.hpp>
#include <pipeline/fork_into.hpp>

using Pipeliner = SAMOSBOR::workflow::Pipeliner;
using PipelineOptions = SAMOSBOR::workflow::PipelineOptions;

using GraphId = SAMOSBOR::assembly::ref::GraphId;
using Assembly = SAMOSBOR::assembly::ref::Assembly;
using AssemblySettings = SAMOSBOR::assembly::ref::AssemblySettings;

namespace core = SAMOSBOR::core;
namespace stages = SAMOSBOR::workflow::stages;

Pipeliner::Pipeliner()
{
	opencascade::handle<Message_Messenger> messenger = Message::DefaultMessenger();
	Message_SequenceOfPrinters printers = Message::DefaultMessenger()->Printers();

	for (auto it = printers.begin(); it != printers.end(); it++)
	{
		messenger->RemovePrinter(*it);
	}
}

core::ResultOr<Assembly> Pipeliner::RunAssemblyPipeline(const AssemblySettings& settings)
{
	const GraphId graphId(settings.graphId);

	stages::ReadStepStage readStepStage(settings);
	stages::BuildAssemblyGraphStage buildGraphStage(graphId);
	stages::BuildAssemblyStage buildAssemblyStage(settings);

	auto basePipeline = readStepStage | buildGraphStage | buildAssemblyStage;

	return basePipeline(graphId);
}

core::Result Pipeliner::RunExportPipeline(const AssemblySettings& settings, const uint32_t pipelineOptions)
{
	bool hasExportStep = pipelineOptions & PipelineOptions::EXPORT_STEP;
	bool hasExportGltf = pipelineOptions & PipelineOptions::EXPORT_GLTF;

	core::ResultOr<Assembly> assemblyPipelineResult = RunAssemblyPipeline(settings);
	if (!assemblyPipelineResult.Ok())
	{
		return assemblyPipelineResult.Res();
	}

	if (hasExportStep)
	{
		stages::WriteStepStage writeStepStage(settings);

		auto res = writeStepStage(assemblyPipelineResult.Value());
		if (!res.Ok())
		{
			return res.Res();
		}
	}

	if (hasExportGltf)
	{
		stages::WriteGltfStage writeGltfStage(settings);
		
		auto res = writeGltfStage(assemblyPipelineResult.Value());
		if (!res.Ok())
		{
			return res.Res();
		}
	}

	return core::Result(core::Result::OK);
}

template <typename T>
core::ResultOr<T> Pipeliner::RunConfigurable(const assembly::AssemblySettings& settings, const uint32_t pipelineOptions)
{
	const GraphId graphId(settings.graphId);

	if (pipelineOptions & PipelineOptions::READ_STEP)
	{
		stages::ReadStepStage readStepStage(settings);
	}
}
