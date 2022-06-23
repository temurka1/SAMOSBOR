#pragma once

namespace SAMOSBOR::step
{
	class Step_Reader;
}

namespace SAMOSBOR::assembly::ref
{
	struct AssemblyGraph;
	struct GraphId;

	/// <summary>
	/// Composes graph for tool assembly encoded in graph id
	/// </summary>
	class AssemblyGraphBuilder final
	{
	public:
		AssemblyGraph Build(const GraphId graphId, const float extensionLength);

		AssemblyGraphBuilder();
		~AssemblyGraphBuilder();
	private:
		step::Step_Reader* _reader;
	};
}

