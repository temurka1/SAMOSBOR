#include "pch.h"
#include "ResultOr.hpp"
#include "GraphId.h"
#include "AssemblyGraph.h"
#include "AssemblyGraphBuilder.h"
#include "AssemblySettings.hpp"
#include "StepData.h"
#include "StepReader.h"

namespace fs = std::filesystem;

namespace core = SAMOSBOR::core;
namespace step = SAMOSBOR::step::ref;
namespace assembly = SAMOSBOR::assembly::ref;

using AssemblyGraph = assembly::AssemblyGraph;
using AssemblyGraphBuilder = assembly::AssemblyGraphBuilder;
using AssemblySettings = assembly::AssemblySettings;

using StepData = step::StepData;
using StepReader = step::StepReader;

namespace
{
	fs::path get_input_path(const fs::path& dataPath)
	{
		if (dataPath.empty())
		{
			return fs::current_path();
		}

		return dataPath;
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

core::ResultOr<AssemblyGraph> AssemblyGraphBuilder::Build(const GraphId& graphId, const AssemblySettings& settings)
{
	std::filesystem::path inputPath = get_input_path(settings.dataPath);

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

		ASSIGN_OR_RETURN_T(const StepData& data, _reader->Read(get_file_id(inputPath, vertex.fileId)), AssemblyGraph);

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
