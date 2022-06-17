// poochcmd.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

using ArgumentParser = argparse::ArgumentParser;
using Assembly_Builder = SAMOSBOR::assembly::Assembly_Builder;

// Usage: 
// SAMOSBORcmd.exe --s assembly.json --o assembly.stp
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
	try
	{
		ArgumentParser program("SAMOSBORcmd");

		program.add_description("Command-line tool to create tool assembly STEP model");

		program.add_argument("--s").help("assembly structure file").required();
		program.add_argument("--o").help("output tool assembly file").required();
		program.add_argument("--l").help("extension length").default_value(0).scan<'i', int>();
		program.add_argument("help");

		program.parse_args(argc, argv);

		Assembly_Builder assemblyBuilder;
		assemblyBuilder.Build(program.get<std::string>("--s"), program.get<std::string>("--o"), program.get<float>("--l"));

	}
	catch (std::runtime_error ex)
	{
		std::cout << ex.what() << std::endl;
	}

	return 0;
}
