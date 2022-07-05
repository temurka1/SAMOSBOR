#include "pch.h"
#include "ResultOr.hpp"
#include "GraphId.h"
#include "AssemblyGraph.h"
#include "AssemblyGraphBuilder.h"
#include "StepData.h"
#include "StepReader.h"

namespace fs = std::filesystem;

namespace core = SAMOSBOR::core;
namespace step = SAMOSBOR::step::ref;
namespace assembly = SAMOSBOR::assembly::ref;

using AssemblyGraph = assembly::AssemblyGraph;
using AssemblyGraphBuilder = assembly::AssemblyGraphBuilder;
using AssemblyGraphSettings = assembly::AssemblyGraphSettings;

using StepData = step::StepData;
using StepReader = step::StepReader;

namespace
{
	fs::path get_input_path(const AssemblyGraphSettings& graphSettings)
	{
		if (graphSettings.inputPath.empty())
		{
			return fs::current_path();
		}

		return graphSettings.inputPath;
	}

	fs::path get_file_id(fs::path inputPath, const std::string_view& fileId)
	{
		fs::path path = inputPath / fileId;
		return path.string() + ".stp";
	}
}

AssemblyGraphBuilder::AssemblyGraphBuilder(): _reader(new StepReader())
{

}

AssemblyGraphBuilder::~AssemblyGraphBuilder()
{
	delete _reader;
}

core::ResultOr<AssemblyGraph> AssemblyGraphBuilder::Build(const GraphId& graphId, const AssemblyGraphSettings& graphSettings)
{
	std::filesystem::path inputPath = get_input_path(graphSettings);

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
		const GraphId::Vertex& vertex = vertices[i];
		const core::ResultOr<StepData> readResult = _reader->Read(get_file_id(inputPath, vertex.fileId));
		
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
	for (size_t i = 1; i < vertices.size(); ++i)
	{
		const uint8_t parentIdx = graph.hierarchy[i];
		const Csw::Port& port = graph.ports[i];

		Csw& parentCsw = graph.csw[parentIdx];

		const CoordinateSystem& mcs = graph.mcs[i];
		const CoordinateSystem* csw = parentCsw.cs.get(parentCsw.ports[port]);

		graph.transforms[i] = core::occ::GetTransform(mcs, *csw);
	}

	return core::ResultOr<AssemblyGraph>(graph);
}
