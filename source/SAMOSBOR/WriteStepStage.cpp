#include "pch.h"
#include "WriteStepStage.h"

#include "Assembly.h"
#include "AssemblySettings.hpp"
#include "StepWriter.h"

#include "EmptyTypeT.hpp"
#include "Result.hpp"

using WriteStepStage = SAMOSBOR::workflow::stages::WriteStepStage;

using Assembly = SAMOSBOR::assembly::ref::Assembly;
using AssemblySettings = SAMOSBOR::assembly::ref::AssemblySettings;

using StepWriter = SAMOSBOR::step::ref::StepWriter;

namespace core = SAMOSBOR::core;

WriteStepStage::WriteStepStage(const AssemblySettings& settings) : 
	Stage<core::EmptyTypeT, Assembly>([&settings](const Assembly& assembly) -> core::ResultOr<core::EmptyTypeT>
	{
		StepWriter stepWriter;
		auto result = stepWriter.Write(settings.outputPath, assembly.GetShapes());

		return core::ResultOr<core::EmptyTypeT>(result);
	})
{
}