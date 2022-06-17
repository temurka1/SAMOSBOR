#pragma once

namespace boost::program_options
{
	class variables_map;
}

class InputOptions
{
public:
	InputOptions(int argc, char* argv[]);
	virtual ~InputOptions();

	float GetExtensionLength() const;

	std::string GetAssemblyOutputFile() const;
	std::string GetAssemblyStructureFile() const;
private:
	boost::program_options::variables_map* _vm;
};


