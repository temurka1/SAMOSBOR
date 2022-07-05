#pragma once

namespace SAMOSBOR::step::ref
{
	class StepReader;
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
		core::ResultOr<AssemblyGraph> Build(const GraphId& graphId, const AssemblySettings& settings);

		AssemblyGraphBuilder();
		~AssemblyGraphBuilder();
	private:
		step::StepReader* _reader;
	};
}

