#include "pch.h"
#include "GraphId.h"
#include "AssemblyGraph.h"
#include "AssemblyGraphBuilder.h"
#include "StepReader.h"

using AssemblyGraph = SAMOSBOR::assembly::ref::AssemblyGraph;
using AssemblyGraphBuilder = SAMOSBOR::assembly::ref::AssemblyGraphBuilder;
using StepReader = SAMOSBOR::step::ref::StepReader;

AssemblyGraphBuilder::AssemblyGraphBuilder(): _reader(new StepReader())
{

}

AssemblyGraphBuilder::~AssemblyGraphBuilder()
{
	delete _reader;
}

AssemblyGraph AssemblyGraphBuilder::Build(const GraphId& graphId, const AssemblyGraphSettings& graphSettings)
{
	AssemblyGraph graph;

	return graph;
}
