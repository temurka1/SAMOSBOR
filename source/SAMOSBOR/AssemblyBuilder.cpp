#include "pch.h"
#include "Result.hpp"
#include "ResultOr.hpp"
#include "Shape.hpp"
#include "StepWriter.h"
#include "GraphId.h"
#include "AssemblyGraph.h"
#include "AssemblySettings.hpp"
#include "AssemblyBuilder.h"
#include "AssemblyGraphBuilder.h"

namespace core = SAMOSBOR::core;
namespace occ = SAMOSBOR::core::occ;

using GraphId = SAMOSBOR::assembly::ref::GraphId;
using AssemblyGraph = SAMOSBOR::assembly::ref::AssemblyGraph;
using AssemblyBuilder = SAMOSBOR::assembly::ref::AssemblyBuilder;
using AssemblyGraphBuilder = SAMOSBOR::assembly::ref::AssemblyGraphBuilder;
using AssemblySettings = SAMOSBOR::assembly::ref::AssemblySettings;

using StepWriter = SAMOSBOR::step::ref::StepWriter;

AssemblyBuilder::AssemblyBuilder(): _graphBuilder(new AssemblyGraphBuilder()), _stepWriter(new StepWriter())
{

}

AssemblyBuilder::~AssemblyBuilder()
{
	delete _graphBuilder;
	delete _stepWriter;
}

core::Result AssemblyBuilder::Build(const std::string_view& graphString, const AssemblySettings& settings)
{
	ASSIGN_OR_RETURN(const AssemblyGraph& graph, _graphBuilder->Build(GraphId(graphString), settings));

	std::vector<TopoDS_Shape> outputShapes;
	outputShapes.reserve(graph.shapes.size());

	// no need to transform root toolitem
	outputShapes.push_back(graph.shapes[0]);

	for (size_t i = 1; i < graph.shapes.size(); i++)
	{
		const occ::Shape& shape = graph.shapes[i];
		const occ::Transform& transform = graph.transforms[i];

		const TopLoc_Location loc(transform.Get());
		outputShapes.push_back(shape.Moved(loc, false));
	}

	return _stepWriter->Write(settings.outputPath, outputShapes);
}
