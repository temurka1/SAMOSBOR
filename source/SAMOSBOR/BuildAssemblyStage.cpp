#include "pch.h"
#include "BuildAssemblyStage.h"

#include "Assembly.h"
#include "AssemblyGraph.h"
#include "AssemblyBuilder.h"
#include "AssemblySettings.hpp"

using BuildAssemblyStage = SAMOSBOR::workflow::stages::BuildAssemblyStage;

using Assembly = SAMOSBOR::assembly::ref::Assembly;
using AssemblyGraph = SAMOSBOR::assembly::ref::AssemblyGraph;
using AssemblyBuilder = SAMOSBOR::assembly::ref::AssemblyBuilder;
using AssemblySettings = SAMOSBOR::assembly::ref::AssemblySettings;

namespace core = SAMOSBOR::core;

BuildAssemblyStage::BuildAssemblyStage(const AssemblySettings& settings) : 
	Stage<Assembly, AssemblyGraph>([&settings](const AssemblyGraph& graph) -> core::ResultOr<Assembly>
	{
		AssemblyBuilder assemblyBuilder;
		return assemblyBuilder.Build(graph, settings);
	})
{
}