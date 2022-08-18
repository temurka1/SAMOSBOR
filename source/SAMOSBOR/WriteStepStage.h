#pragma once
#include "Stage.hpp"

namespace SAMOSBOR::core
{
	struct EmptyTypeT;
}

namespace SAMOSBOR::assembly::ref
{
	class Assembly;
	struct AssemblySettings;
}

namespace SAMOSBOR::workflow::stages
{
	namespace core = SAMOSBOR::core;
	namespace assembly = SAMOSBOR::assembly::ref;

	/// <summary>
	/// Exports tool assembly to STEP
	/// </summary>
	class WriteStepStage : public Stage<core::EmptyTypeT, assembly::Assembly>
	{
	public:
		WriteStepStage(const assembly::AssemblySettings& settings);
	};
}