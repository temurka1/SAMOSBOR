#pragma once

namespace SAMOSBOR::core
{
	template<typename T> class ResultOr;
}

namespace SAMOSBOR::assembly::ref
{
	namespace core = SAMOSBOR::core;

	class Assembly;
	struct AssemblyGraph;
	struct AssemblySettings;

	class AssemblyBuilder
	{
	public:
		core::ResultOr<Assembly> Build(const AssemblyGraph& graph, const AssemblySettings& settings);
	};
}