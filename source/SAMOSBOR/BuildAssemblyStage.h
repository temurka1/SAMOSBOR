#pragma once
#include "Stage.hpp"

namespace SAMOSBOR::assembly::ref
{
	class Assembly;
	struct AssemblyGraph;
	struct AssemblySettings;
}

namespace SAMOSBOR::workflow::stages
{
	namespace assembly = SAMOSBOR::assembly::ref;

	/// <summary>
	/// builds assembly - transforms shapes, triangulate, etc
	/// </summary>
	class BuildAssemblyStage : public Stage<assembly::Assembly, assembly::AssemblyGraph>
	{
	public:
		BuildAssemblyStage(const assembly::AssemblySettings& settings);
	};
}