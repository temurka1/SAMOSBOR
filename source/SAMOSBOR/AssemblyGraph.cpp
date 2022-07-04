#include "pch.h"
#include "AssemblyGraph.h"

using AssemblyGraph = SAMOSBOR::assembly::ref::AssemblyGraph;

AssemblyGraph::AssemblyGraph(const size_t toolItemsCount)
{
	hierarchy.reserve(toolItemsCount);
	transforms.reserve(toolItemsCount);
	shapes.reserve(toolItemsCount);
	pcs.reserve(toolItemsCount);
	mcs.reserve(toolItemsCount);
	csw.reserve(toolItemsCount);
}