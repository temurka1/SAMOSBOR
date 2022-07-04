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

	// * build hierarchy
	// * sort hierarchy in depth-first order - to ensure sequental access pattern
	// * based on hierarchy fill data arrays in assembly graph - shapes, coordinate systems
	// * calculate and fill transfroms (current MCS to parent CSW)
	
	for (size_t i = 0; i < edges.size(); ++i)
	{
		const GraphId::Edge& edge = edges[i];
		graph.hierarchy[edge.to] = edge.from;
	}

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

	for (size_t i = 0; i < vertices.size(); ++i)
	{
		const uint8_t parent = graph.hierarchy[i];

		const CoordinateSystem& mcsCurrent = graph.mcs[i];
		const CoordinateSystem& cswParent = graph.csw[parent].;

		graph.transforms[i] = core::occ::GetTransform(mcsCurrent, cswParent);
	}

	return core::ResultOr<AssemblyGraph>(graph);
}
