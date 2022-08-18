#include "pch.h"
#include "ResultOr.hpp"
#include "GraphId.h"
#include "AssemblyGraph.h"
#include "AssemblyGraphBuilder.h"
#include "StepData.h"

namespace core = SAMOSBOR::core;
namespace step = SAMOSBOR::step::ref;
namespace assembly = SAMOSBOR::assembly::ref;

using AssemblyGraph = assembly::AssemblyGraph;
using AssemblyGraphBuilder = assembly::AssemblyGraphBuilder;

using StepData = step::StepData;

core::ResultOr<AssemblyGraph> AssemblyGraphBuilder::Build(const GraphId& graphId, const std::vector<StepData> stepData)
{
	const std::vector<GraphId::Vertex>& vertices = graphId.Vertices();
	const std::vector<GraphId::Edge>& edges = graphId.Edges();

	AssemblyGraph graph(vertices.size());

	// build the graph hierarchy
	//
	for (size_t i = 0; i < edges.size(); ++i)
	{
		const GraphId::Edge& edge = edges[i];

		graph.hierarchy[edge.to] = edge.from;
		graph.ports[edge.to] = edge.port;
	}

	// fill up the data - shapes, coordinate systems, etc
	//
	for (size_t i = 0; i < vertices.size(); ++i)
	{
		const StepData& data = stepData[i];

		graph.pcs[i] = data.Pcs();
		graph.mcs[i] = data.Mcs();
		graph.csw[i] = data.Csw();

		graph.shapes[i] = data.Shape();
	}

	// calculate and store transforms (current MCS to parent CSW)
	//
	for (size_t i = 1; i < vertices.size(); ++i)
	{
		const uint8_t parentIdx = graph.hierarchy[i];
		const Csw::Port& port = graph.ports[i];

		Csw& parentCsw = graph.csw[parentIdx];

		const CoordinateSystem& mcs = graph.mcs[i];
		const CoordinateSystem* csw = parentCsw.cs.get(parentCsw.ports[port]);

		graph.transforms[i] = core::occ::GetTransform(mcs, *csw).PreMultiply(graph.transforms[parentIdx]);
	}

	return core::ResultOr<AssemblyGraph>(graph);
}
