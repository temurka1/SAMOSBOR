#include "pch.h"
#include "WriteGltfStage.h"

#include "Assembly.h"
#include "AssemblySettings.hpp"

#include "EmptyTypeT.hpp"

using WriteGltfStage = SAMOSBOR::workflow::stages::WriteGltfStage;

using Assembly = SAMOSBOR::assembly::ref::Assembly;
using AssemblySettings = SAMOSBOR::assembly::ref::AssemblySettings;

namespace core = SAMOSBOR::core;

WriteGltfStage::WriteGltfStage(const AssemblySettings& settings): 
	Stage<core::EmptyTypeT, Assembly>([&settings](const Assembly& assembly) -> core::ResultOr<core::EmptyTypeT>
	{
		return core::ResultOr<core::EmptyTypeT>(core::Result(core::Result::OK));
	})
{
}