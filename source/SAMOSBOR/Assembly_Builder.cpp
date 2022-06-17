#include "pch.h"
#include "Assembly_Builder.h"
#include "Assembly_Parser.h"
#include "Assembly_GraphBuilder.h"
#include "Assembly_TransformationVisitor.h"

#include "Step_Reader.h"
#include "Step_Writer.h"

using Assembly_Builder = SAMOSBOR::assembly::Assembly_Builder;
using Assembly_Parser = SAMOSBOR::assembly::Assembly_Parser;
using Assembly_GraphBuilder = SAMOSBOR::assembly::Assembly_GraphBuilder;
using Assembly_TransformationVisitor = SAMOSBOR::assembly::Assembly_TransformationVisitor;

using Step_Reader = SAMOSBOR::step::Step_Reader;
using Step_Writer = SAMOSBOR::step::Step_Writer;

void Assembly_Builder::Build(const std::string& structureFile, const std::string& outputFile, float extensionLength) const
{
	std::vector<std::shared_ptr<TopoDS_Shape>> outShapes;

	Step_Reader stepReader;
	Step_Writer stepWriter;

	Assembly_Parser assemblyParser;
	Assembly_GraphBuilder graphBuilder(&stepReader);
	Assembly_TransformationVisitor transformationVisitor(&outShapes);

	auto parsedItems = assemblyParser.Parse(structureFile);
	auto graph = graphBuilder.Build(parsedItems, extensionLength);

	boost::depth_first_search(graph, boost::visitor(transformationVisitor));

	stepWriter.Write(outputFile, outShapes);
}
