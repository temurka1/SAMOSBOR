#include "pch.h"
#include "Input.h"
#include <stdexcept>

using namespace option;

Input::Input(int argc, char* argv[])
{
	usage = new Descriptor[]
	{
		{ OptionIndex::FILES, 0, "f", "files", Arg::Optional, "--files, -f  \tList of STEP files to process." },
		{ OptionIndex::EXTENSION_LENGTH, 1, "l", "length", Arg::Optional, "--length, -l  \tExtension length between tool items." },
		{ OptionIndex::OUTPUT, 2, "o", "output", Arg::Optional, "--output, -o  \tOutput file name." },
	};

	stats = new Stats(usage, argc - (argc > 0), argv + (argc > 0));
	options = new Option[stats->buffer_max];
	buffer = new Option[stats->buffer_max];
	parser = new Parser(usage, argc - (argc > 0), argv + (argc > 0), options, buffer);
}

Input::~Input()
{
	delete stats;
	delete parser;
	delete[] options;
	delete[] buffer;
	delete[] usage;
}

std::vector<Standard_CString> Input::GetInputFiles() const
{
	std::vector<Standard_CString> files;

	if (!parser->error())
	{
		for (option::Option* opt = options[OptionIndex::FILES]; opt; opt = opt->next())
		{
			files.push_back(opt->arg);
		}
	}

	return files;
}

float Input::GetExtensionLength() const
{
	if (!parser->error())
	{
		auto opt = options[OptionIndex::EXTENSION_LENGTH];
		return opt.arg != nullptr ? atof(opt.arg) : 0.0f;
	}
	
	return 0.0f;
}

Standard_CString Input::GetOutputFile() const
{
	if (!parser->error())
	{
		return options[OptionIndex::OUTPUT].arg;
	}

	return "";
}
