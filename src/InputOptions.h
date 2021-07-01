#pragma once

#include <boost/program_options.hpp>

namespace pooch::input
{
	class InputOptions
	{
	public:
		InputOptions(int argc, char* argv[]);
		virtual ~InputOptions();

		Standard_Real GetExtensionLength() const;

		std::string GetAssemblyOutputFile() const;
		std::string GetAssemblyStructureFile() const;
	private:
		boost::program_options::variables_map _vm;
	};
}

