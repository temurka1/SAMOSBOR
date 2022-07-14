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
namespace occt = opencascade;

using GraphId = SAMOSBOR::assembly::ref::GraphId;
using AssemblyGraph = SAMOSBOR::assembly::ref::AssemblyGraph;
using AssemblyBuilder = SAMOSBOR::assembly::ref::AssemblyBuilder;
using AssemblyGraphBuilder = SAMOSBOR::assembly::ref::AssemblyGraphBuilder;
using AssemblySettings = SAMOSBOR::assembly::ref::AssemblySettings;

using StepWriter = SAMOSBOR::step::ref::StepWriter;

namespace
{
	__forceinline void silence_occt()
	{
		occt::handle<Message_Messenger> messenger = Message::DefaultMessenger();
		Message_SequenceOfPrinters printers = Message::DefaultMessenger()->Printers();

		for (auto it = printers.begin(); it != printers.end(); it++)
		{
			messenger->RemovePrinter(*it);
		}
	}

	__forceinline void transform_shapes(const AssemblyGraph& graph, std::vector<TopoDS_Shape>* output)
	{	
		// no need to transform root toolitem
		output->push_back(graph.shapes[0]);
		
		for (size_t i = 1; i < graph.shapes.size(); i++)
		{
			const occ::Shape& shape = graph.shapes[i];
			const occ::Transform& transform = graph.transforms[i];

			const TopLoc_Location loc(transform.Get());
			output->push_back(shape.Moved(loc, false));
		}
	}
}

AssemblyBuilder::AssemblyBuilder(bool silenceOcctMessages): _graphBuilder(new AssemblyGraphBuilder()), _stepWriter(new StepWriter())
{
	if (silenceOcctMessages)
	{
		silence_occt();
	}
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

	transform_shapes(graph, &outputShapes);

	return _stepWriter->Write(settings.outputPath, outputShapes);
}
