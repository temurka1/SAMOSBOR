#include "pch.h"
#include "AssemblyGraph.h"

using AssemblyGraph = SAMOSBOR::assembly::ref::AssemblyGraph;

AssemblyGraph::AssemblyGraph(const size_t toolItemsCount)
{
	hierarchy.resize(toolItemsCount, 0);
	ports.resize(toolItemsCount, 0);

	transforms.resize(toolItemsCount);
	shapes.resize(toolItemsCount);
	pcs.resize(toolItemsCount);
	mcs.resize(toolItemsCount);
	csw.resize(toolItemsCount);
}