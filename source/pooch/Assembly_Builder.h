#pragma once

namespace pooch::assembly
{
	class Assembly_Builder
	{
	public:
		void Build(const std::string& structureFile, const std::string& outputFile, float extensionLength) const;
	};
}

