// Pooch.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include "Input.h"

#include "Step_Data.h"
#include "Step_Parser.h"
#include "Step_Exporter.h"
#include "Step_Transformer.h"

#pragma comment (lib, "TKernel.lib")
#pragma comment (lib, "TKXSBase.lib")
#pragma comment (lib, "TKSTEPBase.lib")
#pragma comment (lib, "TKSTEP.lib")
#pragma comment (lib, "TKG3d.lib")
#pragma comment (lib, "TKMath.lib")

//
// Usage:
// ./pooch.exe -f 264181.stp -f 281314.stp -l 20 -o assembly.stp
// -f defines input files (one "-f" per file)
// -o defines ouput assembly file
// -l defines extension length
//
int main(int argc, char* argv[])
{
	Input input(argc, argv);

	auto inputFiles = input.GetInputFiles();
	auto outputFile = input.GetOutputFile();
	auto extensionLength = input.GetExtensionLength();

	Step_Parser parser;
	Step_Transformer transformer;
	Step_Exporter exporter;

	try
	{
		auto data = parser.Parse(inputFiles);
		transformer.Transform(data, extensionLength);
		exporter.Export(outputFile, data);
	}
	catch (std::exception ex)
	{
		std::cout << ex.what() << std::endl;
	}

	return 0;
}
