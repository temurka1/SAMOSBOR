#pragma once
#include <filesystem>

namespace SAMOSBOR::core
{
	class Result;
}

namespace SAMOSBOR::step::ref
{
	namespace fs = std::filesystem;
	namespace core = SAMOSBOR::core;

	class StepWriter
	{
	public:
		core::Result Write(const fs::path& output, const std::vector<TopoDS_Shape>& data);
	};
}

