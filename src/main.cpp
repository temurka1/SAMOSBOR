// Pooch.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include "InputOptions.h"

#include "Step_Reader.h"
#include "Step_Writer.h"

#include "Assembly_Parser.h"
#include "Assembly_GraphBuilder.h"

#pragma comment (lib, "TKernel.lib")
#pragma comment (lib, "TKXSBase.lib")
#pragma comment (lib, "TKSTEPBase.lib")
#pragma comment (lib, "TKSTEP.lib")
#pragma comment (lib, "TKG3d.lib")
#pragma comment (lib, "TKMath.lib")

int main(int argc, char* argv[])
{
	pooch::input::InputOptions input(argc, argv);

	pooch::step::Step_Reader stepReader;
	pooch::step::Step_Writer stepWriter;

	pooch::assembly::Assembly_Parser assemblyParser;
	pooch::assembly::Assembly_GraphBuilder graphBuilder(stepReader);

	try
	{
		auto outputFile = input.GetAssemblyOutputFile();
		auto structureFile = input.GetAssemblyStructureFile();
		auto extensionLength = input.GetExtensionLength();

		auto parsedItems = assemblyParser.Parse(structureFile);
		auto graph = graphBuilder.Build(parsedItems, extensionLength);
	}
	catch (std::runtime_error ex)
	{
		std::cout << ex.what() << std::endl;
	}

	return 0;
}
