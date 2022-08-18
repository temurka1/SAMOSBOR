#pragma once
#include "Stage.hpp"
#include <vector>

namespace SAMOSBOR::assembly::ref
{
	struct GraphId;
	struct AssemblyGraph;
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
	/// builds assembly graph
	/// </summary>
	class BuildAssemblyGraphStage : public Stage<assembly::AssemblyGraph, std::vector<step::StepData>>
	{
	public:
		BuildAssemblyGraphStage(const assembly::GraphId& graphId);
	};
}
