#pragma once
#include "Stage.hpp"

namespace SAMOSBOR::assembly::ref
{
	struct GraphId;
	struct AssemblySettings;
}

namespace SAMOSBOR::step::ref
{
	class StepData;
}

namespace SAMOSBOR::workflow::stages
{
	namespace core = SAMOSBOR::core;
	namespace assembly = SAMOSBOR::assembly::ref;
	namespace step = SAMOSBOR::step::ref;

	/// <summary>
	/// Reads data from input STEP files
	/// </summary>
	class ReadStepStage : public Stage<std::vector<step::StepData>, assembly::GraphId>
	{
	public:
		ReadStepStage(const assembly::AssemblySettings& settings);
	};
}