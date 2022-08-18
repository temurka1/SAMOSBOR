#include "pch.h"
#include "BuildAssemblyGraphStage.h"

#include "StepData.h"
#include "GraphId.h"
#include "AssemblyGraph.h"
#include "AssemblyGraphBuilder.h"

using BuildAssemblyGraphStage = SAMOSBOR::workflow::stages::BuildAssemblyGraphStage;

using GraphId = SAMOSBOR::assembly::ref::GraphId;
using AssemblyGraph = SAMOSBOR::assembly::ref::AssemblyGraph;
using AssemblyGraphBuilder = SAMOSBOR::assembly::ref::AssemblyGraphBuilder;

using StepData = SAMOSBOR::step::ref::StepData;

namespace core = SAMOSBOR::core;
namespace workflow = SAMOSBOR::workflow;

BuildAssemblyGraphStage::BuildAssemblyGraphStage(const GraphId& graphId) : 
	Stage<AssemblyGraph, std::vector<StepData>>([&graphId](const std::vector<StepData>& stepData) -> core::ResultOr<AssemblyGraph>
	{
		AssemblyGraphBuilder builder;
		return builder.Build(graphId, stepData);
	})
{
}
