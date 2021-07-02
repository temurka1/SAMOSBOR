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
#pragma comment (lib, "TKTopAlgo.lib")
#pragma comment (lib, "TKBRep.lib")

// Usage: 
// pooch.exe --s assembly.json --o assembly.stp
// --s filename.json - file which describes assembly structure
// --o filename.stp - output tool assembly file
// --l 40 - extension length (0 by default)

// Assembly structure file scheme:
// id - some unique assembly part identifier. I used guids, but maybe there is more suitable data type (I thought about Order Number but wasn't sure it will be uniqie enought)
// parent_id - id of assembly part's parent
// file - filename of assembly part
// csw_slots - array of names of CSW. Part can be mounted to multiple csw slots, in that case it will be internally copied
// 
// 
//{
//	"data":
//	[
//		{
//			"id": "38929E97-D5B6-4071-A851-A9E6A5C3964F",
//			"parent_id" : "00000000-0000-0000-0000-000000000000",
//			"file" : "15107_10.stp",
//			"csw_slots" : []
//		},
//		{
//			"id": "4DA09192-E9B9-4EF5-947B-E1727BBDB994",
//			"parent_id" : "38929E97-D5B6-4071-A851-A9E6A5C3964F",
//			"file" : "20_71.stp",
//			"csw_slots" :
//			[
//				"CSW"
//			]
//		}
//	]
//}

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
