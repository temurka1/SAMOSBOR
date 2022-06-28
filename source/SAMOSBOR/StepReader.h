#pragma once
#include "ResultOr.hpp"

namespace SAMOSBOR::step::ref
{
	/// <summary>
	/// 
	/// </summary>
	struct StepFileData
	{
	public:
		StepFileData(const STEPControl_Reader& reader);
	private:
		STEPControl_Reader _reader;
	};

	/// <summary>
	/// 
	/// </summary>
	struct StepReader
	{
	 	core::ResultOr<StepFileData> Read(const std::string& filename);
	};
}
