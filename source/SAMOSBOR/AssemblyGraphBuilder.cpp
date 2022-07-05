#include "pch.h"
#include "ResultOr.hpp"
#include "GraphId.h"
#include "AssemblyGraph.h"
#include "AssemblyGraphBuilder.h"
#include "StepData.h"
#include "StepReader.h"

namespace core = SAMOSBOR::core;
namespace step = SAMOSBOR::step::ref;
namespace assembly = SAMOSBOR::assembly::ref;

using AssemblyGraph = assembly::AssemblyGraph;
using AssemblyGraphBuilder = assembly::AssemblyGraphBuilder;
using AssemblyGraphSettings = assembly::AssemblyGraphSettings;

using StepData = step::StepData;
using StepReader = step::StepReader;

AssemblyGraphBuilder::AssemblyGraphBuilder(): _reader(new StepReader())
{

}

AssemblyGraphBuilder::~AssemblyGraphBuilder()
{
	delete _reader;
}

core::ResultOr<AssemblyGraph> AssemblyGraphBuilder::Build(const GraphId& graphId, const AssemblyGraphSettings& graphSettings)
{
	const std::vector<GraphId::Vertex>& vertices = graphId.Vertices();
	const std::vector<GraphId::Edge>& edges = graphId.Edges();

	AssemblyGraph graph(vertices.size());

	// build the graph hierarchy
	//
	for (size_t i = 0; i < edges.size(); ++i)
	{
		const GraphId::Edge& edge = edges[i];

		graph.hierarchy.parents[edge.to] = edge.from;
		graph.hierarchy.ports[edge.to] = edge.port;
	}

	// fill up the data - shapes, coordinate systems, etc
	//
	for (size_t i = 0; i < vertices.size(); ++i)
	{
		const GraphId::Vertex& vertex = vertices[i];
		const core::ResultOr<StepData> readResult = _reader->Read(vertex.fileId);
		
		if (readResult.Ok())
		{
			graph.pcs[i] = readResult.Value().Pcs();
			graph.mcs[i] = readResult.Value().Mcs();
			graph.csw[i] = readResult.Value().Csw();

			graph.shapes[i] = readResult.Value().Shape();
		}
		else
		{
			return core::ResultOr<AssemblyGraph>(readResult.Res());
		}
	}

	// calculate and store transforms (current MCS to parent CSW)
	//
	for (size_t i = 0; i < vertices.size(); ++i)
	{
		const uint8_t parentIdx = graph.hierarchy.parents[i];

		Csw& csw = graph.csw[parentIdx];

		const Csw::Port& port = graph.hierarchy.ports[parentIdx];
		const Csw::Key& key = csw.ports[port];

		const CoordinateSystem& mcsCurrent = graph.mcs[i];
		const CoordinateSystem* cswParent = csw.cs.get(key);

		graph.transforms[i] = core::occ::GetTransform(mcsCurrent, *cswParent);
	}

	return core::ResultOr<AssemblyGraph>(graph);
}
