#pragma once

namespace SAMOSBOR::core
{
	class Result;
}

namespace SAMOSBOR::step::ref
{
	class StepWriter;
}

namespace SAMOSBOR::assembly::ref
{
	namespace core = SAMOSBOR::core;
	namespace step = SAMOSBOR::step::ref;

	struct AssemblySettings;
	class AssemblyGraphBuilder;

	class AssemblyBuilder
	{
	public:
		AssemblyBuilder(bool silenceOcctMessages = true);
		~AssemblyBuilder();

		core::Result Build(const std::string_view& graphString, const AssemblySettings& settings);
	private:
		AssemblyGraphBuilder* _graphBuilder;
		step::StepWriter* _stepWriter;
	};
}