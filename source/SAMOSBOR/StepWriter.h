#pragma once
#include <filesystem>

namespace SAMOSBOR::core
{
	class Result;
}

class STEPControl_Writer;

namespace SAMOSBOR::step::ref
{
	namespace fs = std::filesystem;
	namespace core = SAMOSBOR::core;

	class StepWriter final
	{
	public:
		StepWriter();
		~StepWriter();

		core::Result Write(const fs::path& output, const std::vector<TopoDS_Shape>& data);
	private:
		STEPControl_Writer* _writer;
	};
}

