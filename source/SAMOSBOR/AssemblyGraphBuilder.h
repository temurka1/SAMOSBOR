#pragma once

namespace SAMOSBOR::step::ref
{
	class StepReader;
}

namespace SAMOSBOR::assembly::ref
{
	struct AssemblyGraph;
	struct GraphId;

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
		AssemblyGraph Build(const GraphId& graphId, const AssemblyGraphSettings& graphSettings);

		AssemblyGraphBuilder();
		~AssemblyGraphBuilder();
	private:
		step::ref::StepReader* _reader;
	};
}

