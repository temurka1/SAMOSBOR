#pragma once
#include <filesystem>

namespace SAMOSBOR::core
{
	template<class T> class ResultOr;
}

namespace SAMOSBOR::step::ref
{
	class StepData;

	/// <summary>
	/// Reads STEP file
	/// </summary>
	class StepReader
	{
	public:
		core::ResultOr<StepData> Read(const std::filesystem::path& filepath, bool printCheckload = false);
	};
}
