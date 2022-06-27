#include "pch.h"
#include "GraphId.h"
#include "AssemblyGraph.h"
#include "AssemblyGraphBuilder.h"
#include "Step_Reader.h"

using AssemblyGraph = SAMOSBOR::assembly::ref::AssemblyGraph;
using AssemblyGraphBuilder = SAMOSBOR::assembly::ref::AssemblyGraphBuilder;
using Step_Reader = SAMOSBOR::step::Step_Reader;

AssemblyGraphBuilder::AssemblyGraphBuilder(): _reader(new Step_Reader())
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
