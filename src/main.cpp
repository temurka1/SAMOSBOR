// Pooch.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include "InputOptions.h"

#include "Step_Reader.h"
#include "Step_Writer.h"

#include "Assembly_Parser.h"
#include "Assembly_GraphBuilder.h"
#include "Assembly_TransformationVisitor.h"

#pragma comment (lib, "TKernel.lib")
#pragma comment (lib, "TKXSBase.lib")
#pragma comment (lib, "TKSTEPBase.lib")
#pragma comment (lib, "TKSTEP.lib")
#pragma comment (lib, "TKG3d.lib")
#pragma comment (lib, "TKMath.lib")

int main(int argc, char* argv[])
{
	std::vector<std::shared_ptr<TopoDS_Shape>> outShapes;

	pooch::input::InputOptions input(argc, argv);

	pooch::step::Step_Reader stepReader;
	pooch::step::Step_Writer stepWriter;

	pooch::assembly::Assembly_Parser assemblyParser;
	pooch::assembly::Assembly_GraphBuilder graphBuilder(stepReader);
	pooch::assembly::Assembly_TransformationVisitor transformationVisitor(&outShapes);

	try
	{
		auto outputFile = input.GetAssemblyOutputFile();
		auto structureFile = input.GetAssemblyStructureFile();
		auto extensionLength = input.GetExtensionLength();

		auto parsedItems = assemblyParser.Parse(structureFile);
		auto graph = graphBuilder.Build(parsedItems, extensionLength);

		boost::depth_first_search(graph, boost::visitor(transformationVisitor));

		stepWriter.Write(outputFile, outShapes);
	}
	catch (std::runtime_error ex)
	{
		std::cout << ex.what() << std::endl;
	}

	return 0;
}
