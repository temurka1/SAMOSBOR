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

	struct GraphId;
	struct AssemblyGraph;

	struct AssemblyGraphSettings
	{
		float extensionLength;
	};

	/// <summary>
	/// Composes graph for tool assembly encoded in graph id
	/// </summary>
	class AssemblyGraphBuilder final
	{
	public:
		core::ResultOr<AssemblyGraph> Build(const GraphId& graphId, const AssemblyGraphSettings& graphSettings);

		AssemblyGraphBuilder();
		~AssemblyGraphBuilder();
	private:
		step::ref::StepReader* _reader;
	};
}

