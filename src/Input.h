#pragma once

#include <vector>
#include <Standard_TypeDef.hxx>

#include "optionparser.h"

enum OptionIndex
{
	FILES,
	EXTENSION_LENGTH,
	OUTPUT
};

class Input
{
public:
	Input(int argc, char* argv[]);
	virtual ~Input();

	std::vector<Standard_CString> GetInputFiles() const;
	float GetExtensionLength() const;
	Standard_CString GetOutputFile() const;
private:
	option::Descriptor* usage;
	option::Stats*  stats;
	option::Option* options;
	option::Option* buffer;
	option::Parser* parser;
};

