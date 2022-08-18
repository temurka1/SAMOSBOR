#pragma once

namespace SAMOSBOR::step::ref
{
	class StepData;
}

namespace SAMOSBOR::core
{
	template<class T> class ResultOr;
}

namespace SAMOSBOR::assembly::ref
{
	namespace core = SAMOSBOR::core;
	namespace step = SAMOSBOR::step::ref;

	struct GraphId;
	struct AssemblyGraph;
	struct AssemblySettings;

	/// <summary>
	/// Composes graph for tool assembly encoded in graph id
	/// </summary>
	class AssemblyGraphBuilder final
	{
	public:
		core::ResultOr<AssemblyGraph> Build(const GraphId& graphId, const std::vector<step::StepData> stepData);
	};
}

